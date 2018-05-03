//ECE-6122: Advanced Programming Techniques
//Beer Pong Game
//Najwa Harif, Sarah Mauret, Hugo Blech, Lucas Chaulan, Elliot Hallais

#pragma once

#include <iostream> // For debug
#include "Sample.h"
#include "Player.h"

namespace Urho3D
{
    class Node;
    class Scene;
}

/// Empty scene with the Skybox component for setting up an unmoving sky
class standaloneScene : public Sample
{
    URHO3D_OBJECT(standaloneScene, Sample);

public:
    /// Construct.
    explicit standaloneScene(Context* context);

    /// Setup after engine initialization and before running the main loop.
    void Start() override;

protected:
    SharedPtr<Node> ballNode_;
    Player player;
    
private:
    /// Construct the scene content.
    void CreateScene();
    /// Set up a viewport for displaying the scene.
    void SetupViewport();
    /// Subscribe to application-wide logic update and post-render update events.
    void SubscribeToEvents();
    /// Read input and moves the camera.
    void MoveCamera(float timeStep);
    /// Read input and moves the ball.
    void MoveBall(float timeStep);
    /// Spawn a physics object from the camera position.
    // void SpawnObject();
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);

    /// Displays cups on the table
    void DisplayCups(Node* tableNode);
};
