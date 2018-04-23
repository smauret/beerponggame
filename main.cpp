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

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Urho2D/Sprite2D.h>


#include "main.h"


#include <Urho3D/DebugNew.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/ToolTip.h>
#include <Urho3D/UI/Text.h>


#include <iostream>
//#include <cTime>
#include <ctime>
#include <vector>
#include <Urho3D/UI/Font.h>
#include "Vec3.h"



// Number of main to draw
static const unsigned NUM_main = 2;

// Custom variable identifier for storing sprite velocity within the UI element
static const StringHash VAR_VELOCITY("Velocity");


URHO3D_DEFINE_APPLICATION_MAIN(main)

main::main(Context* context) :
        Sample(context),
        dragBeginPosition_(IntVector2::ZERO),
        BeginPosition_(IntVector2::ZERO)
{
}

void main::Start()
{
    Sample::Start();

    // Hook up to the frame update events
   // SubscribeToEvents();
    // Najwa's push
    InitControls();

    // Set the mouse mode to use in the sample
    Sample::InitMouseMode(MM_FREE);
}
void main::CreatedraggableBall()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();

    // Create a draggable Ball button
    Button* draggableBall = new Button(context_);
    draggableBall->SetTexture(cache->GetResource<Texture2D>("Textures/ball.png")); // Set texture
    draggableBall->SetBlendMode(BLEND_ALPHA);
    draggableBall->SetSize(128, 128);
    draggableBall->SetPosition(3*(graphics->GetWidth() - draggableBall->GetWidth()) / 4, 200);
    draggableBall->SetName("Ball");
    ui->GetRoot()->AddChild(draggableBall);

    // Add a tooltip to Ball button
    ToolTip* toolTip = new ToolTip(context_);
    draggableBall->AddChild(toolTip);
    toolTip->SetPosition(IntVector2(draggableBall->GetWidth() + 5, draggableBall->GetWidth() / 2)); // slightly offset from close button

    // Subscribe draggableBall to Drag Events (in order to make it draggable)
    // See "Event list" in documentation's Main Page for reference on available Events and their eventData
    SubscribeToEvent(draggableBall, E_DRAGBEGIN, URHO3D_HANDLER(main, HandleDragBegin));
    SubscribeToEvent(draggableBall, E_DRAGMOVE, URHO3D_HANDLER(main, HandleDragMove));
    SubscribeToEvent(draggableBall, E_DRAGEND, URHO3D_HANDLER(main, HandleDragEnd));
}
void main::HandleDragBegin(StringHash eventType, VariantMap& eventData)
{
    startTime = std::clock(); //Start timer
    // Get UIElement relative position where input (touch or click) occurred (top-left = IntVector2(0,0))
    dragBeginPosition_ = IntVector2(eventData["ElementX"].GetInt(), eventData["ElementY"].GetInt());
    BeginPosition_ = IntVector2(eventData["X"].GetInt(), eventData["Y"].GetInt());
}


void main::HandleDragMove(StringHash eventType, VariantMap& eventData)
{
    IntVector2 dragCurrentPosition = IntVector2(eventData["X"].GetInt(), eventData["Y"].GetInt());
  //  UIElement* draggedElement = static_cast<UIElement*>(eventData["Element"].GetPtr());

    draggedElement_ = static_cast<UIElement*>(eventData["Element"].GetPtr());
    draggedElement_ ->SetPosition(dragCurrentPosition - dragBeginPosition_);
    draggedElement_ ->SetSize(dragCurrentPosition.y_/4, dragCurrentPosition.y_/4);

}

void main::HandleDragEnd(StringHash eventType, VariantMap& eventData)
{
    IntVector2 dragCurrentPosition = IntVector2(eventData["X"].GetInt(), eventData["Y"].GetInt());
    // Calculate the power based on the time and the distance
    double speed = GetSpeed(BeginPosition_,dragCurrentPosition);
    std::cout << "Speed : " << speed << std::endl;
    // Calculate the rotation angle 
    double rotation_angle = GetRotation(BeginPosition_,dragCurrentPosition);
    std::cout << "Rotation angle in degrees : " << rotation_angle* 180.0 / M_PI << std::endl;

    // Calculate trajectory
    IntVector3 finalPositionCm = GetInitPosCm(dragCurrentPosition);
    vector<Vec3<int>> ballTrajectory = lucas_.throwBall(M_PI/4, rotation_angle, (float)(finalPositionCm.y_), speed*100, (float)(finalPositionCm.x_));
    for (int i=0; i<ballTrajectory.size(); i++) {
        graphicsTrajectory_.emplace_back(1024/2, 768/2,0);
    }
    lucas_.get_z_graphics(ballTrajectory, graphicsTrajectory_);
    lucas_.get_x_graphics(ballTrajectory, graphicsTrajectory_);
    /*for(int i=0;i<graphicsTrajectory_.size();i++) {
        std::cout << "x : " << graphicsTrajectory_[i].getX() << " | z : " << graphicsTrajectory_[i].getZ() << std::endl;
    } */

    UI* ui = GetSubsystem<UI>();
    //ui->GetRoot()->RemoveChild(main_[0]);

   // draggedElement_ = static_cast<UIElement*>(eventData["Element"].GetPtr());
    k=0;

    SubscribeToEvents();

}

double main::GetSpeed(IntVector2 initPos2, IntVector2 endPos2) {
    double distance = sqrt((initPos2.x_  - endPos2.x_)*(initPos2.x_  - endPos2.x_) + (initPos2.y_  - endPos2.y_)*(initPos2.y_  - endPos2.y_));
    double distance_cm = distance * 0.027;//64583333;
    clock_t endTime = std::clock();
    double timePassed_sec = (endTime - startTime) / ((double)CLOCKS_PER_SEC/10);
    std::cout << "Time passed : " << timePassed_sec << std::endl;
    double speed = distance_cm/timePassed_sec;
    return speed;
}



double main::GetRotation(IntVector2 initPos2, IntVector2 endPos2) {
    double sizeVec = sqrt((initPos2.x_ - endPos2.x_)*(initPos2.x_ - endPos2.x_) + (initPos2.y_ - endPos2.y_)*(initPos2.y_ - endPos2.y_));
  //  std::cout << "Taille vecteur : " << sizeVec << std::endl;
    double rotation = acos((endPos2.x_-initPos2.x_)/sizeVec);
    std::cout << "Rotation angle in radians : " << rotation << std::endl;
    return rotation;
}

// Function to translate a point in graphics (pixels) to a point on the table (cm)
// Assumption : 90° view
IntVector3 main::GetInitPosCm(IntVector2 initPos){
    // Size of the table in cm
    int widthCm = 60;
    // Width of the window in pixels
    int widthPix = 1023;
    int heightPix = 768;
    int heightCm = 100;
    IntVector3 initPosCm = IntVector3(0,0,0);
    // xA = xG * 59/1023
    initPosCm.x_= initPos.x_ * (widthCm-1)/(widthPix - 1);
    // zA = (767 - zG) * 99/767
    initPosCm.z_ = ((heightPix - 1) - initPos.y_)*(heightCm - 1)/(heightPix - 1);
    // yA = tan(90 - 45)*sqrt(xA^2 + zA^2)
    initPosCm.y_ = tan(M_PI/4) * sqrt(initPosCm.x_*initPosCm.x_ + initPosCm.y_*initPosCm.y_);
    return initPosCm;
}

void main::InitControls() {
    UI* ui = GetSubsystem<UI>();
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
    // Create the Window and add it to the UI's root node
    Window* window_ = new Window(context_);
    ui->GetRoot()->AddChild(window_);

    // Set the loaded style as default style
    ui->GetRoot()->SetDefaultStyle(style);

    // Set Window size and layout settings
    window_->SetMinWidth(384);
    window_->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
    window_->SetAlignment(HA_CENTER, VA_CENTER);
    window_->SetName("Window");
    window_->SetStyleAuto();

    // Create a Button
    Button *button = new Button(context_);
    // Add controls to Window
    window_->AddChild(button);
    button->SetName("Button");
    button->SetMinHeight(24);
    // Set the text displayed on the button
    Font* font = cache->GetResource<Font>("Fonts/Anonymous Pro.ttf");
    Text* buttonText = button->CreateChild<Text>();
    buttonText->SetFont(font, 12);
    buttonText->SetAlignment(HA_CENTER, VA_CENTER);
    buttonText->SetText("PLAY");
    // Apply previously set default style
    button->SetStyleAuto();
    Color* c = new Color(1.0,0.0,0.0,1.0);
    button->SetColor(*c);
    button->SetBringToFront(true);

    // Display background image
    Graphics* graphics = GetSubsystem<Graphics>();
    // Get rendering window size as floats
    float width = (float)graphics->GetWidth();
    float height = (float)graphics->GetHeight();
    Texture2D* background = cache->GetResource<Texture2D>("Textures/background_beer.jpg");
    SharedPtr<BorderImage> back(new BorderImage(context_));
    ui->GetRoot()->AddChild(back);
    back->SetTexture(background);
    back->SetSize(width,height);
    back->SetBringToBack(true);
    back->SetOpacity(0.3);

    // Title of the first page
    Text* title = new Text(context_);
    title->SetText("Welcome to the BeerPong Game !");
    ui->GetRoot()->AddChild(title);
    title->SetStyleAuto();
    title->SetOpacity(1.0);
    title->SetFont("Fonts/Anonymous Pro.ttf",30);
    title->SetPosition(((int)width-title->GetSize().x_)/2,(int)height/3);

    SubscribeToEvent(button, E_RELEASED, URHO3D_HANDLER(main,HandlePlayPressed));

}

void main::HandleReturnPressed(StringHash eventType, VariantMap& eventData)
{
    UI* ui = GetSubsystem<UI>();
    ui->GetRoot()->RemoveAllChildren();
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
    // Create the Window and add it to the UI's root node
    Window* window_ = new Window(context_);
    ui->GetRoot()->AddChild(window_);

    // Set the loaded style as default style
    ui->GetRoot()->SetDefaultStyle(style);

    // Set Window size and layout settings
    window_->SetMinWidth(384);
    window_->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
    window_->SetAlignment(HA_CENTER, VA_CENTER);
    window_->SetName("Window");
    window_->SetStyleAuto();

    // Create a Button
    Button *button = new Button(context_);
    // Add controls to Window
    window_->AddChild(button);
    button->SetName("Button");
    button->SetMinHeight(24);
    // Set the text displayed on the button
    Font* font = cache->GetResource<Font>("Fonts/Anonymous Pro.ttf");
    Text* buttonText = button->CreateChild<Text>();
    buttonText->SetFont(font, 12);
    buttonText->SetAlignment(HA_CENTER, VA_CENTER);
    buttonText->SetText("PLAY");
    // Apply previously set default style
    button->SetStyleAuto();
    Color* c = new Color(1.0,0.0,0.0,1.0);
    button->SetColor(*c);
    button->SetBringToFront(true);

    // Display background image
    Graphics* graphics = GetSubsystem<Graphics>();
    // Get rendering window size as floats
    float width = (float)graphics->GetWidth();
    float height = (float)graphics->GetHeight();
    Texture2D* background = cache->GetResource<Texture2D>("Textures/background_beer.jpg");
    SharedPtr<BorderImage> back(new BorderImage(context_));
    ui->GetRoot()->AddChild(back);
    back->SetTexture(background);
    back->SetSize(width,height);
    back->SetBringToBack(true);
    back->SetOpacity(0.3);

    // Title of the first page
    Text* title = new Text(context_);
    title->SetText("Welcome to the BeerPong Game !");
    ui->GetRoot()->AddChild(title);
    title->SetStyleAuto();
    title->SetOpacity(1.0);
    title->SetFont("Fonts/Anonymous Pro.ttf",30);
    title->SetPosition(((int)width-title->GetSize().x_)/2,(int)height/3);

    SubscribeToEvent(button, E_RELEASED, URHO3D_HANDLER(main,HandlePlayPressed));
}


void main::HandlePlayPressed(StringHash eventType, VariantMap& eventData)
{
    // Create player
    lucas_ = Player("Lucas",10);


    // Graphics
    UI* ui = GetSubsystem<UI>();
    ui->GetRoot()->RemoveAllChildren();
    Createmain();
    CreatedraggableBall();
}
void main::Createmain()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();

    // Get rendering window size as floats
    float width = (float)graphics->GetWidth();
    float height = (float)graphics->GetHeight();

    // Get the cup texture
    Texture2D* decalTex = cache->GetResource<Texture2D>("Textures/beer.png");

    for (unsigned i = 0; i < NUM_main; ++i)
    {
        // Create a new sprite, set it to use the texture
        SharedPtr<Sprite> sprite(new Sprite(context_));
        sprite->SetTexture(decalTex);

        // Set position of the cup
        sprite->SetPosition(Vector2((width+i*100)/2,(height+i*100)/2));

        // Set sprite size & hotspot in its center
        sprite->SetSize(IntVector2(128, 128));
        sprite->SetHotSpot(IntVector2(64, 64));

        // Set additive blending mode
        sprite->SetBlendMode(BLEND_ALPHA);

        // Add as a child of the root UI element
        ui->GetRoot()->AddChild(sprite);
        //std::cout << "Position of cup " << i << " : " << ui->GetRoot()->FindChild(sprite) << std::endl;

        // Store sprite's velocity as a custom variable
        sprite->SetVar(VAR_VELOCITY, Vector2(Random(200.0f) - 100.0f, Random(200.0f) - 100.0f));

        // Store main to our own container for easy movement update iteration
        main_.Push(sprite);
    }

    //Create return button
    XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
    // Create the Window and add it to the UI's root node
    Window* window_ = new Window(context_);
    ui->GetRoot()->AddChild(window_);

    // Set the loaded style as default style
    ui->GetRoot()->SetDefaultStyle(style);

    // Set Window size and layout settings
    window_->SetMinWidth(100);
    window_->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
  //  window_->SetAlignment(HA_CENTER, VA_CENTER);
    window_->SetAlignment(HA_LEFT, VA_TOP);
    window_->SetName("Window");
    window_->SetStyleAuto();

    // Create a Button
    Button *buttonReturn = new Button(context_);
    // Add controls to Window
    window_->AddChild(buttonReturn);
    buttonReturn->SetName("ButtonReturn");
    buttonReturn->SetMinHeight(24);
    // Set the text displayed on the button
    Font* font = cache->GetResource<Font>("Fonts/Anonymous Pro.ttf");
    Text* buttonText = buttonReturn->CreateChild<Text>();
    buttonText->SetFont(font, 12);
    buttonText->SetAlignment(HA_CENTER, VA_CENTER);
    buttonText->SetText("RETURN");
    // Apply previously set default style
    buttonReturn->SetStyleAuto();
    Color* c = new Color(1.0,0.0,0.0,1.0);
    buttonReturn->SetColor(*c);
    buttonReturn->SetBringToFront(true);
    SubscribeToEvent(buttonReturn, E_RELEASED, URHO3D_HANDLER(main,HandleReturnPressed));



}


void main::SubscribeToEvents()
{
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(main, HandleUpdate));
}

void main::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;
    // Take the frame time step, which is stored as a float
    //float timeStep = eventData[P_TIMESTEP].GetFloat();

    if(k<graphicsTrajectory_.size()){
        draggedElement_->SetPosition(graphicsTrajectory_[k].getX(), graphicsTrajectory_[k].getZ());
       // draggedElement_->SetSize(ballTrajectory_[k].getZ()*50,ballTrajectory_[k].getZ()*50);
        k=k+1;
    } else {
        std::cout << "YES" << std::endl;
        draggedElement_->SetPosition(1024/2, 768/2);
        //   UnsubscribeFromEvent(E_UPDATE, HandleUpdate);
        UnsubscribeFromEvent(E_UPDATE);
    }
    // Move main, scale movement with time step
    // Movemain(timeStep);
}
