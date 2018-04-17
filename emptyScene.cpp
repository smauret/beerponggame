#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Camera.h>
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

#include "emptyScene.h"

#include <Urho3D/DebugNew.h>


/// Made from Sample 11_Physics

/// This is a simple example to create a basic scene. It has a sky and a floor. That's it.

/// First objective: reduce the amount necessary to make a simple scene.
/// Final objective: do not rely (or modify) Sample.h simplifier class, as it does
/// things we do not want (add the logo, and remove control over some features as
/// the window title)



URHO3D_DEFINE_APPLICATION_MAIN(emptyScene)

emptyScene::emptyScene(Context* context) :
    Sample(context)
{
}

void emptyScene::Start()
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
    Sample::InitMouseMode(MM_RELATIVE);
}


void emptyScene::CreateScene()
{
    auto* cache = GetSubsystem<ResourceCache>();

    scene_ = new Scene(context_);

    // Create octree, use default volume (-1000, -1000, -1000) to (1000, 1000, 1000)
    // Create a physics simulation world with default parameters, which will update at 60fps.
    // The Octree must exist before creating drawable components
    scene_->CreateComponent<Octree>();

    // Create a Zone component for ambient lighting
    Node* zoneNode = scene_->CreateChild("Zone");
    auto* zone = zoneNode->CreateComponent<Zone>();
    zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
    zone->SetAmbientColor(Color(0.15f, 0.15f, 0.15f));

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


    // Create a floor object, 1000 x 1000 world units. Adjust position so that the ground is at zero Y
    Node* floorNode = scene_->CreateChild("Floor");
    floorNode->SetPosition(Vector3(0.0f, -0.5f, 0.0f));
    floorNode->SetScale(Vector3(1000.0f, 1.0f, 1000.0f));
    auto* floorObject = floorNode->CreateComponent<StaticModel>();
    floorObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    floorObject->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));

    // Create a table object, 1x1x1 world units. Have a high Y so that we can see it
    Node* tableNode = scene_->CreateChild("Table");
    // TODO: We need to modify the table object so that the referential is easy to place on the world !!
    tableNode->SetPosition(Vector3(10.0f, 3.0f, 10.0f));
    tableNode->SetScale(Vector3(6.0f, 6.0f, 6.0f));
    auto* tableObject = tableNode->CreateComponent<StaticModel>();
    tableObject->SetModel(cache->GetResource<Model>("Models/Table.mdl"));
    tableObject->SetMaterial(cache->GetResource<Material>("Materials/Table.xml"));
    // TODO: Fix texture, comes and goes when I turn around

    // Add cup on table
    // TODO: modify the Cup object too !
    Node* cupNode = tableNode->CreateChild("Cup");
    cupNode->SetPosition(Vector3(0.1f, 1.1f, 0.1f));
    cupNode->SetScale(Vector3(0.1f, 1.0f, 0.1f));
    auto* cupObject = cupNode->CreateComponent<StaticModel>();
    cupObject->SetModel(cache->GetResource<Model>("Models/Krujka.mdl"));
    cupObject->SetMaterial(cache->GetResource<Material>("Materials/Krujka-Me.xml"));

    // Create the camera. Set far clip to match the fog. Note: now we actually create the camera node outside the scene, because
    // we want it to be unaffected by scene load / save
    cameraNode_ = new Node(context_);
    auto* camera = cameraNode_->CreateComponent<Camera>();
    camera->SetFarClip(500.0f);

    // Set an initial position for the camera scene node above the floor
    cameraNode_->SetPosition(Vector3(0.0f, 5.0f, 0.0f));
}

void emptyScene::SetupViewport()
{
    auto* renderer = GetSubsystem<Renderer>();

    // Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);
}

void emptyScene::SubscribeToEvents()
{
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(emptyScene, HandleUpdate));
}

// Should be renamed to UpdateScene if the scene evolves too in this method
void emptyScene::MoveCamera(float timeStep)
{
    // Do not move if the UI has a focused element (the console)
    if (GetSubsystem<UI>()->GetFocusElement())
    return;

    auto* input = GetSubsystem<Input>();

    // Movement speed as world units per second
    const float MOVE_SPEED = 20.0f;
    // Mouse sensitivity as degrees per pixel
    const float MOUSE_SENSITIVITY = 0.1f;

    // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
    // TODO: We should restrict this for our example
    IntVector2 mouseMove = input->GetMouseMove();
    yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
    pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
    pitch_ = Clamp(pitch_, -90.0f, 90.0f);

    // Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
    // TODO: If drunk, the player may have a changing roll / changing effects
    cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));

    // Read ZQSD keys and move the camera scene node to the corresponding direction if they are pressed
    if (input->GetKeyDown(KEY_Z))
    cameraNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);

    if (input->GetKeyDown(KEY_S))
    cameraNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);

    if (input->GetKeyDown(KEY_Q))
    cameraNode_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);

    if (input->GetKeyDown(KEY_D))
    cameraNode_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);

}

void emptyScene::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Move the camera, scale movement with time step
    MoveCamera(timeStep);
}
