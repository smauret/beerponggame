//ECE-6122: Advanced Programming Techniques
//Beer Pong Game
//Najwa Harif, Sarah Mauret, Hugo Blech, Lucas Chaulan, Elliot Hallais

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>

#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/Zone.h>

#include <Urho3D/Input/Input.h>
#include <Urho3D/IO/File.h>
#include <Urho3D/IO/FileSystem.h>

#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/Constraint.h>

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>

#include <Urho3D/UI/Window.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>

#include "standaloneScene.h"

#include <Urho3D/DebugNew.h>

#include <ctime>
#include <iostream> // For debug



URHO3D_DEFINE_APPLICATION_MAIN(standaloneScene)

standaloneScene::standaloneScene(Context* context) :
    Sample(context)
{
}

void standaloneScene::Start()
{
    // Execute base class startup
    Sample::Start();

    // Create the scene content
    CreateScene();

    CreateInstructions(ball_velocity);

    // Setup the viewport for displaying the scene
    SetupViewport();

    // Hook up to the frame update and render post-update events
    SubscribeToEvents();

    // Set the mouse mode to use in the sample
    Sample::InitMouseMode(MM_FREE);
}



void standaloneScene::CreateScene()
{
    auto* cache = GetSubsystem<ResourceCache>();

    scene_ = new Scene(context_);

    scene_->CreateComponent<Octree>();
    scene_->CreateComponent<PhysicsWorld>();

    // Create a Zone component for ambient lighting & fog control
    Node* zoneNode = scene_->CreateChild("Zone");
    auto* zone = zoneNode->CreateComponent<Zone>();
    zone->SetBoundingBox(BoundingBox(-100.0f, 100.0f));
    zone->SetAmbientColor(Color(0.15f, 0.15f, 0.15f));
    // fog, diminishes Field Of View (disabled for now)
    zone->SetFogColor(Color(1.0f, 1.0f, 1.0f));
    zone->SetFogStart(300.0f);
    zone->SetFogEnd(500.0f);

    // Create a directional light to the world. Enable cascaded shadows on it
    Node* lightNode = scene_->CreateChild("DirectionalLight");
    lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
    auto* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
    // shadows
    light->SetCastShadows(true);
    light->SetShadowBias(BiasParameters(0.00025f, 0.5f));
    // Set cascade splits at 10, 50 and 200 world units, fade shadows out at 80% of maximum shadow distance
    light->SetShadowCascade(CascadeParameters(10.0f, 50.0f, 200.0f, 0.0f, 0.8f));

    // Skybox
    Node* skyNode = scene_->CreateChild("Sky");
    skyNode->SetScale(500.0f); // The scale actually does not matter
    auto* skybox = skyNode->CreateComponent<Skybox>();
    skybox->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    skybox->SetMaterial(cache->GetResource<Material>("Materials/Skybox.xml"));


    // Create a floor object, 1000 x 1000 world units. Adjust position so that the ground is at zero Y
    Node* floorNode = scene_->CreateChild("Floor");
    floorNode->SetPosition(Vector3(0.0f, -1.0f, 0.0f));
    floorNode->SetScale(Vector3(1000.0f, 1.0f, 1000.0f));
    auto* floorObject = floorNode->CreateComponent<StaticModel>();
    floorObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    // floorObject->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));
    floorObject->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));


    RigidBody* floorBody = floorNode->CreateComponent<RigidBody>();
    auto* floorShape = floorNode->CreateComponent<CollisionShape>();
    floorShape->SetBox(Vector3::ONE);

    // Create a table object
    // Node* tableNode = scene_->CreateChild("Table");
    tableNode = scene_->CreateChild("Table");
    tableNode->SetPosition(Vector3(0.0f, 1.0f, 0.0f));
    tableNode->SetScale(Vector3(0.6f, 1.0f, 1.8f));
    auto* tableObject = tableNode->CreateComponent<StaticModel>();
    tableObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    tableObject->SetMaterial(cache->GetResource<Material>("Materials/BPTable.xml"));

    RigidBody* tableBody = floorNode->CreateComponent<RigidBody>();
    tableBody->SetMass(0.0f);
    tableBody->SetFriction(0.01f);
    auto* tableShape = floorNode->CreateComponent<CollisionShape>();
    tableShape->SetBox(Vector3::ONE);
    // tableNode->SetPosition(Vector3(0.0f, 1.0f, 0.0f));

    // Define the current player
    Vec2f tableSize = Vec2f();
    tableSize.x = tableNode->GetScale().x_/2.0f;
    tableSize.y = tableNode->GetScale().z_/2.0f/2.0f;
    player = Player("Player 1", 6, tableSize);

    // Display cups
    DisplayCups();


    // Create ball object
    ballNode_ = scene_->CreateChild("Ball");
    ballNode_->SetScale(Vector3(0.03f, 0.03f, 0.03f));
    ballNode_->SetPosition(Vector3(0.0f, 1.60f, -1.0f));
    auto* ballObject_ = ballNode_->CreateComponent<StaticModel>();
    ballObject_->SetModel(cache->GetResource<Model>("Models/Sphere.mdl"));
    ballObject_->SetMaterial(cache->GetResource<Material>("Materials/BPBall.xml"));
    ballObject_->SetCastShadows(true);

    auto* ballBody = ballNode_->CreateComponent<RigidBody>();
    // ballBody->SetMass(1.0f);
    // ballBody->SetFriction(0.75f);
    auto* ballShape = ballNode_->CreateComponent<CollisionShape>();
    ballShape->SetBox(Vector3::ONE);

    // Create the camera. Set far clip to match the fog. Note: now we actually create the camera node outside the scene, because
    // we want it to be unaffected by scene load / save
    cameraNode_ = new Node(context_);
    auto* camera = cameraNode_->CreateComponent<Camera>();
    camera->SetFarClip(100.0f);

    // Set/Get the characteristics of the camera
    // camera->SetFov(30.0f);
    std::cout << "Camera FOV: " << camera->GetFov() << std::endl;
    std::cout << "Camera Zoom: " << camera->GetZoom() << std::endl;

    // Set an initial position for the camera scene node above the floor
    cameraNode_->SetPosition(Vector3(0.0f, 1.80f, - tableNode->GetScale().z_/2 - 0.5f));
}

void standaloneScene::CreateInstructions(float& ball_velocity)
{
    auto* cache = GetSubsystem<ResourceCache>();
    auto* ui = GetSubsystem<UI>();

    ui->GetRoot()->RemoveAllChildren();

    // Construct new Text object, set string to display and font to use
    auto* instructionText = ui->GetRoot()->CreateChild<Text>();
    std::string message;
    message = std::string("Use A-D keys to move, LMB to place ball\nSpace to launch ball\nKey up and down to change ball velocity\nCurrent ball velocity: ") + std::to_string(ball_velocity) +'\n';
    instructionText->SetText(message.c_str());
    instructionText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 13);
    // The text has multiple rows. Center them in relation to each other
    instructionText->SetTextAlignment(HA_CENTER);

    // Position the text relative to the screen center
    instructionText->SetHorizontalAlignment(HA_CENTER);
    instructionText->SetVerticalAlignment(VA_CENTER);
    instructionText->SetPosition(0, ui->GetRoot()->GetHeight() / 3);
}

void standaloneScene::SetupViewport()
{
    // auto* cache = GetSubsystem<ResourceCache>();
    auto* renderer = GetSubsystem<Renderer>();

    renderer->SetHDRRendering(true);

    // Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);

}

void standaloneScene::DisplayCups()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    vector<Cup> cups = player.getCups();
    std::cout << "Cup vector size: " << cups.size() << std::endl;

    for (unsigned i = 0; i < cups.size(); ++i)
    {
        // Add 1 cup on table
        Node* cupNode = tableNode->CreateChild("Cup");
        std::cout << "Placing cup at " << cups[i].getPosition().x << ", " << cups[i].getPosition().y << std::endl;
        cupNode->SetScale(Vector3(cups[i].getRadius(), cups[i].getHeight(), cups[i].getRadius()/2));
        cupNode->SetPosition(Vector3(cups[i].getPosition().x, tableNode->GetScale().y_/2 + cupNode->GetScale().y_/2, cups[i].getPosition().y));
        auto* cupObject = cupNode->CreateComponent<StaticModel>();
        cupObject->SetModel(cache->GetResource<Model>("Models/Cylinder.mdl"));
        cupObject->SetMaterial(cache->GetResource<Material>("Materials/BPCup.xml"));
        cupObject->SetCastShadows(true);

        RigidBody* cupBody = cupNode->CreateComponent<RigidBody>();
        auto* cupShape = cupNode->CreateComponent<CollisionShape>();
        cupShape->SetBox(Vector3::ONE);

    }

}

/// Registration to scene events
void standaloneScene::SubscribeToEvents()
{
    SubscribeToEvent(E_MOUSEBUTTONDOWN, URHO3D_HANDLER(standaloneScene,HandleMouse));
    // SubscribeToEvent(ballNode_, E_NODECOLLISION, URHO3D_HANDLER(standaloneScene, HandleNodeCollision));
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(standaloneScene, HandleUpdate));
}

/// Updates are managed here
void standaloneScene::HandleMouse(StringHash eventType, VariantMap& eventData)
{
    /**
    *   The user can select the initial position of the ball by clicking on the screen
    */
    int screen_x = GetSubsystem<Input>()->GetMousePosition().x_;
    int screen_y = GetSubsystem<Input>()->GetMousePosition().y_;

    float x_c = cameraNode_->GetPosition().x_;
    float y_c = cameraNode_->GetPosition().y_;
    float z_c = cameraNode_->GetPosition().z_;

    float delta_z = 0.5f;
    auto* camera = cameraNode_->GetComponent<Camera>();
    float new_x = delta_z*tan(camera->GetFov()/2)*(float(screen_x)/768.0f - 0.5f) + x_c - ballNode_->GetScale().x_/2.0f;
    float new_y = - delta_z*tan(camera->GetFov()/2)*(float(screen_y)/1024.0f - 0.5f) + y_c - ballNode_->GetScale().x_/2.0f;

    ballNode_->SetPosition(Vector3(new_x, new_y, z_c + delta_z));

}

void standaloneScene::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Move the camera, scale movement with time step
    MoveCamera(timeStep);
    CreateInstructions(ball_velocity);
}

// void standaloneScene::HandleNodeCollision(StringHash eventType, VariantMap& eventData)
// {
//     auto* otherNode = eventData["OtherNode"].GetPtr();
//     auto* otherBody = eventData["OtherBody"].GetPtr();
//     VectorBuffer contacts = eventData["Contacts"].GetBuffer();
//
//     std::cout << "Collision with " << otherNode->GetName() << std::endl;
//     while (!contacts.eof)
//     {
//         Vector3 contactPosition = contacts.ReadVector3();
//         Vector3 contactNormal = contacts.ReadVector3();
//         float contactDistance = contacts.ReadFloat();
//         float contactImpulse = contacts.ReadFloat();
//         std::cout << "\t at (" << contactPosition.x_ <<", "<<contactPosition.y_ <<", "<<contactPosition.z_ <<")"<<std::endl;
//     }
// }


void standaloneScene::MoveCamera(float timeStep)
{
    // Do not move if the UI has a focused element (the console)
    if (GetSubsystem<UI>()->GetFocusElement())
    return;

    auto* input = GetSubsystem<Input>();

    // Movement speed as world units per second
    const float MOVE_SPEED = 0.75f;

    // Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
    cameraNode_->SetRotation(Quaternion(0.0f, 0.0f, 0.0f));

    // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    if (input->GetKeyDown(KEY_A))
    {
        if (cameraNode_->GetPosition().x_ > -1.5f)
        {
            cameraNode_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
        }
    }
    if (input->GetKeyDown(KEY_D))
    {
        if (cameraNode_->GetPosition().x_ < 1.5f)
        {
            cameraNode_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);
        }
    }

    // Set velocity
    if(input->GetKeyDown(KEY_UP))
    {
        ball_velocity += 0.01f;
    }
    if(input->GetKeyDown(KEY_DOWN))
    {
        ball_velocity -= 0.01f;
    }

    //Launch ball
    if(input->GetKeyPress(KEY_SPACE))
    {
        auto* ballBody = ballNode_->GetComponent<RigidBody>();
        ballBody->SetMass(0.05f);
        ballBody->SetLinearVelocity(Vector3(0.0f, 0.5f, 0.5f)*ball_velocity);
    }

    // Stop ball
    if(ballNode_->GetPosition().z_ > 2.5f || ballNode_->GetComponent<RigidBody>()->GetLinearVelocity().Length() < 0.1f) //ballNode_->GetPosition().y_ < 1.0f
    {
        auto* ballBody = ballNode_->GetComponent<RigidBody>();
        ballBody->SetMass(0.0f);
        ballBody->SetLinearVelocity(Vector3(0.0f, 0.0f, 0.0f));
        SendEvent(E_MOUSEBUTTONDOWN);
    }

}
