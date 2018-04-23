//
// Copyright (c) 2008-2017 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "Sample.h"
#include "Player.h"



/// Moving mainNajwa example.
/// This sample demonstrates:
///     - Adding Sprite elements to the UI
///     - Storing custom data (sprite velocity) inside UI elements
///     - Handling frame update events in which the mainNajwa are moved
class mainNajwa : public Sample
{
    // Enable type information.
    URHO3D_OBJECT(mainNajwa, Sample);

public:
    /// Construct.
    mainNajwa(Context* context);

    /// Setup after engine initialization and before running the main loop.
    virtual void Start();

protected:
    /// Return XML patch instructions for screen joystick layout for a specific sample app, if any.
    virtual String GetScreenJoystickPatchString() const { return
        "<patch>"
        "    <add sel=\"/element/element[./attribute[@name='Name' and @value='Hat0']]\">"
        "        <attribute name=\"Is Visible\" value=\"false\" />"
        "    </add>"
        "</patch>";
    }

private:
    /// Construct the mainNajwa.
    void CreatemainNajwa();
    /// Move the mainNajwa using the delta time step given.
    void MovemainNajwa(float timeStep);
    /// Subscribe to application-wide logic update events.
    void SubscribeToEvents();
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    void InitControls();
    void CreateDraggableFish();
    void HandleDragBegin(StringHash eventType, VariantMap& eventData);
    /// Handle drag move for the fish button.
    void HandleDragMove(StringHash eventType, VariantMap& eventData);
    /// Handle drag end for the fish button.
    void HandleDragEnd(StringHash eventType, VariantMap& eventData);

    void HandlePlayPressed(StringHash eventType, VariantMap& eventData);
    double GetRotation(IntVector2 initPosition, IntVector2 endPosition);
    double GetSpeed(IntVector2 initPos2, IntVector2 endPos2);
    IntVector3 GetInitPosCm(IntVector2 initPos);

        /// Vector to store the mainNajwa for iterating through them.
    Vector<SharedPtr<Sprite> > mainNajwa_;
    IntVector2 dragBeginPosition_;
    IntVector2 BeginPosition_;
    clock_t startTime;
    Player lucas_;
    UIElement* draggedElement_;
    vector<Vec3<int>> graphicsTrajectory_;
    int k=0;


};
