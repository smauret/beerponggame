#pragma once

#include "Sample.h"

namespace Urho3D
{
    class Node;
    class Scene;
}

/// Empty scene with the Skybox component and a floor
class emptyScene : public Sample
{
    URHO3D_OBJECT(emptyScene, Sample);

public:
    /// Construct.
    explicit emptyScene(Context* context);

    /// Setup after engine initialization and before running the main loop.
    void Start() override;

private:
    /// Construct the scene content.
    void CreateScene();
    /// Set up a viewport for displaying the scene.
    void SetupViewport();
    /// Subscribe to application-wide logic update and post-render update events.
    void SubscribeToEvents();
    /// Read input and moves the camera.
    void MoveCamera(float timeStep);
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);

};
