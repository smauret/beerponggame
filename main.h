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

class main : public Sample
{
    // Enable type information.
URHO3D_OBJECT(main, Sample);

public:
    /// Construct.
    main(Context* context);

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
    /** Create the main page of the game : Cups , table, score window, background */
    void InitBoardGame();

    /// Subscribe to application-wide logic update events.
    void SubscribeToEvents();


    /** Create the welcome page of the game : Background, play window and button, title */
    void InitWelcomePage();
    /** Create the draggable ping pong ball */
    void CreatedraggableBall();
    /** Create a return button for the main page of the game */
    void CreateReturnButton();

    /// Events handlers
    /** Handler of screen update event */
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    /** Handler of beginning of the pinpong ball drag */
    void HandleDragBegin(StringHash eventType, VariantMap& eventData);
    /** Handler of the drag move of the pinpong ball */
    void HandleDragMove(StringHash eventType, VariantMap& eventData);
    /** Handler of the drag end of the pinpong ball */
    void HandleDragEnd(StringHash eventType, VariantMap& eventData);
    /** Handler of the return button of the main page */
    void HandleReturnPressed(StringHash eventType, VariantMap& eventData);
    /** Handler of the play button of the welcome page */
    void HandlePlayPressed(StringHash eventType, VariantMap& eventData);
    /** Handler of the mouse click to move the ball at the mouse position */
    void HandleMouse(StringHash eventType, VariantMap& eventData);

    /**
     * Calculate the rotation of the movement
     * @param initPos2 : initial position of the ball (pix)
     * @param endPos2 : end position of the ball (pix)
     * @return : angle of the movement
     */
    double GetRotation(IntVector2 initPos2, IntVector2 endPos2){
        double sizeVec = sqrt((initPos2.x_ - endPos2.x_)*(initPos2.x_ - endPos2.x_) + (initPos2.y_ - endPos2.y_)*(initPos2.y_ - endPos2.y_));
        //  std::cout << "Taille vecteur : " << sizeVec << std::endl;
        double rotation = acos((endPos2.x_-initPos2.x_)/sizeVec);
        std::cout << "Rotation angle in radians : " << rotation << std::endl;
        return rotation;
    }

    /**
     * Calculate the speed of the movement
     * @param initPos2 : initial position of the ball (pix)
     * @param endPos2 : end position of the ball (pix)
     * @return : speed of the movement
     * TODO : Normaliser entre 2 valeurs (demander aux archi)
     */
    double GetSpeed(IntVector2 initPos2, IntVector2 endPos2){
        double distance = sqrt((initPos2.x_  - endPos2.x_)*(initPos2.x_  - endPos2.x_) + (initPos2.y_  - endPos2.y_)*(initPos2.y_  - endPos2.y_));
        double distance_cm = distance;//64583333;
        clock_t endTime = clock();
        double timePassed_sec = (endTime - startTime) / ((double)CLOCKS_PER_SEC/10);
        std::cout << "Time passed : " << timePassed_sec << std::endl;
        double speed = distance_cm/timePassed_sec;
        return speed;
    };

    /**
     * Translate the position from pixels to cm
     * @param initPos : initial position (pix)
     * @return : initial position (cm)
     */
    IntVector3 GetInitPosCm(IntVector2 initPos);

    /**
     * Modify the text window to announce score
     * @param cupScored
     */
    void ThrowResult(int cupScored);

    /// Vector to store the main for iterating through them.
    Vector<SharedPtr<Sprite> > main_;
    Vector<SharedPtr<UIElement> > uielem_;

    IntVector2 dragBeginPosition_;
    IntVector2 BeginPosition_;
    clock_t startTime;
    Player lucas_;
    UIElement* draggedElement_;
    vector<Vec3<int>> graphicsTrajectory_;
    int k=0;


};