#pragma once

#include "Sample.h"

namespace Urho3D
{
    class Node;
    class Scene;
}

/// Empty scene with the Skybox component for setting up an unmoving sky
class simpleScene : public Sample
{
    URHO3D_OBJECT(simpleScene, Sample);

public:
    /// Construct.
    explicit simpleScene(Context* context);

    /// Setup after engine initialization and before running the main loop.
    void Start() override;


private:
    /// Construct the scene content.
    void CreateScene();
    /// Construct an instruction text to the UI.
    // void CreateInstructions();
    /// Set up a viewport for displaying the scene.
    void SetupViewport();
    /// Subscribe to application-wide logic update and post-render update events.
    void SubscribeToEvents();
    /// Read input and moves the camera.
    void MoveCamera(float timeStep);
    /// Spawn a physics object from the camera position.
    // void SpawnObject();
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    /// Handle the post-render update event.
    void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);

};
