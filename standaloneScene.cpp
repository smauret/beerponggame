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

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>

#include "standaloneScene.h"

#include <Urho3D/DebugNew.h>

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

    // Create octree, use default volume (-1000, -1000, -1000) to (1000, 1000, 1000)
    // Create a physics simulation world with default parameters, which will update at 60fps. Like the Octree must
    // exist before creating drawable components, the PhysicsWorld must exist before creating physics components.
    // Finally, create a DebugRenderer component so that we can draw physics debug geometry
    scene_->CreateComponent<Octree>();
    // scene_->CreateComponent<PhysicsWorld>();

    // Create a Zone component for ambient lighting & fog control
    Node* zoneNode = scene_->CreateChild("Zone");
    auto* zone = zoneNode->CreateComponent<Zone>();
    zone->SetBoundingBox(BoundingBox(-100.0f, 100.0f));
    zone->SetAmbientColor(Color(0.15f, 0.15f, 0.15f));
    // fog, diminishes Field Of View (disabled for now)
    // zone->SetFogColor(Color(1.0f, 1.0f, 1.0f));
    // zone->SetFogStart(300.0f);
    // zone->SetFogEnd(500.0f);

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

    // Create skybox. The Skybox component is used like StaticModel, but it will be always located at the camera, giving the
    // illusion of the box planes being far away. Use just the ordinary Box model and a suitable material, whose shader will
    // generate the necessary 3D texture coordinates for cube mapping
    Node* skyNode = scene_->CreateChild("Sky");
    skyNode->SetScale(500.0f); // The scale actually does not matter
    auto* skybox = skyNode->CreateComponent<Skybox>();
    skybox->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    skybox->SetMaterial(cache->GetResource<Material>("Materials/Skybox.xml"));

    //Make an axis system
    Node* axisNode = scene_->CreateChild("Axis");
    axisNode->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    axisNode->SetScale(1.0f);
    auto* axis = axisNode->CreateComponent<StaticModel>();
    axis->SetModel(cache->GetResource<Model>("Models/Editor/Axes.mdl"));
    axis->SetMaterial(cache->GetResource<Material>("Materials/Editor/BlueUnlit.xml"));


    // Create a floor object, 1000 x 1000 world units. Adjust position so that the ground is at zero Y
    Node* floorNode = scene_->CreateChild("Floor");
    floorNode->SetPosition(Vector3(0.0f, -1.0f, 0.0f));
    floorNode->SetScale(Vector3(1000.0f, 1.0f, 1000.0f));
    auto* floorObject = floorNode->CreateComponent<StaticModel>();
    floorObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    floorObject->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));

    // Make the floor physical by adding RigidBody and CollisionShape components. The RigidBody's default
    // parameters make the object static (zero mass.) Note that a CollisionShape by itself will not participate
    // in the physics simulation
    RigidBody* floorBody = floorNode->CreateComponent<RigidBody>();
    auto* floorShape = floorNode->CreateComponent<CollisionShape>();
    // Set a box shape of size 1 x 1 x 1 for collision. The shape will be scaled with the scene node scale, so the
    // rendering and physics representation sizes should match (the box model is also 1 x 1 x 1.)
    floorShape->SetBox(Vector3::ONE);

    // We need to modify the table object so that the referential is easy to place on the world !!
    // Create a table object, 1x1x1 world units. Have a high Y so that we can see it
    Node* tableNode = scene_->CreateChild("Table");
    tableNode->SetPosition(Vector3(0.0f, 1.0f, 0.0f));
    tableNode->SetScale(Vector3(0.6f, 1.0f, 2.4f));
    auto* tableObject = tableNode->CreateComponent<StaticModel>();
    tableObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    tableObject->SetMaterial(cache->GetResource<Material>("Materials/Jack.xml"));

    RigidBody* tableBody = floorNode->CreateComponent<RigidBody>();
    auto* tableShape = floorNode->CreateComponent<CollisionShape>();
    tableShape->SetBox(Vector3::ONE);


    // Add 1 cup on table
    Node* cupNode = tableNode->CreateChild("Cup");
    cupNode->SetScale(Vector3(0.1f, 0.12f/2, 0.1f/2));
    cupNode->SetPosition(Vector3(0.0f, tableNode->GetScale().y_/2 + cupNode->GetScale().y_/2, 0.0f));
    auto* cupObject = cupNode->CreateComponent<StaticModel>();
    cupObject->SetModel(cache->GetResource<Model>("Models/Cylinder.mdl"));
    cupObject->SetMaterial(cache->GetResource<Material>("Materials/BPCup.xml"));
    cupObject->SetCastShadows(true);

    // Add 1 ball in the air
    ballNode_ = scene_->CreateChild("Ball");
    ballNode_->SetScale(Vector3(0.02f, 0.02f, 0.02f));
    ballNode_->SetPosition(Vector3(0.0f, 1.60f, -1.0f));
    auto* ballObject_ = ballNode_->CreateComponent<StaticModel>();
    ballObject_->SetModel(cache->GetResource<Model>("Models/Sphere.mdl"));
    ballObject_->SetMaterial(cache->GetResource<Material>("Materials/BPBall.xml"));
    ballObject_->SetCastShadows(true);

    // Create RigidBody and CollisionShape components like above. Give the RigidBody mass to make it movable
    // and also adjust friction. The actual mass is not important; only the mass ratios between colliding
    // objects are significant
    // auto* body = boxNode->CreateComponent<RigidBody>();
    // body->SetMass(1.0f);
    // body->SetFriction(0.75f);
    // auto* shape = boxNode->CreateComponent<CollisionShape>();
    // shape->SetBox(Vector3::ONE);


    // Create the camera. Set far clip to match the fog. Note: now we actually create the camera node outside the scene, because
    // we want it to be unaffected by scene load / save
    cameraNode_ = new Node(context_);
    auto* camera = cameraNode_->CreateComponent<Camera>();
    camera->SetFarClip(100.0f);

    // Set/Get the characteristics of the camera
    // camera->SetFov(40.0f);
    std::cout << "Camera FOV: " << camera->GetFov() << std::endl;
    std::cout << "Camera Zoom: " << camera->GetZoom() << std::endl;

    // Set an initial position for the camera scene node above the floor
    cameraNode_->SetPosition(Vector3(0.0f, 1.80f, - tableNode->GetScale().z_/2 - 0.5f));
}

void standaloneScene::SetupViewport()
{
    auto* renderer = GetSubsystem<Renderer>();

    // Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);
}

/// Registration to scene events
void standaloneScene::SubscribeToEvents()
{
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(standaloneScene, HandleUpdate));
}

/// Updates are managed here
void standaloneScene::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Move the camera, scale movement with time step
    MoveCamera(timeStep);
    MoveBall(timeStep);
}

void standaloneScene::MoveCamera(float timeStep)
{
    // Do not move if the UI has a focused element (the console)
    if (GetSubsystem<UI>()->GetFocusElement())
    return;

    auto* input = GetSubsystem<Input>();

    // Movement speed as world units per second
    const float MOVE_SPEED = 1.25f;
    // Mouse sensitivity as degrees per pixel
    // const float MOUSE_SENSITIVITY = 0.1f;

    // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
    // IntVector2 mouseMove = input->GetMouseMove();
    // yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
    // pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
    // pitch_ = Clamp(pitch_, -90.0f, 90.0f);

    // Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
    cameraNode_->SetRotation(Quaternion(10.0f, 0.0f, 0.0f));

    // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    // We should restrict this for our example
    // if (input->GetKeyDown(KEY_Z)){
    //     cameraNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
    // }
    // if (input->GetKeyDown(KEY_S))
    // {
    //     cameraNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
    // }
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


}

void standaloneScene::MoveBall(float timeStep)
{
    // Do not move if the UI has a focused element (the console)
    if (GetSubsystem<UI>()->GetFocusElement())
    return;

    auto* input = GetSubsystem<Input>();

    // Movement speed as world units per second
    const float MOVE_SPEED = 0.75f;

    if (input->GetKeyDown(KEY_Z)){
        ballNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
    }
    if (input->GetKeyDown(KEY_S))
    {
        ballNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
    }


}