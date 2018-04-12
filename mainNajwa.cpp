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


#include "mainNajwa.h"


#include <Urho3D/DebugNew.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/ToolTip.h>
#include <Urho3D/UI/Text.h>


#include <iostream>
#include <cTime>
#include <vector>
#include <Urho3D/UI/Font.h>


// Number of mainNajwa to draw
static const unsigned NUM_mainNajwa = 2;

// Custom variable identifier for storing sprite velocity within the UI element
static const StringHash VAR_VELOCITY("Velocity");

URHO3D_DEFINE_APPLICATION_MAIN(mainNajwa)

mainNajwa::mainNajwa(Context* context) :
        Sample(context),
        dragBeginPosition_(IntVector2::ZERO),
        BeginPosition_(IntVector2::ZERO)
{
}

void mainNajwa::Start()
{
    Sample::Start();

    // Hook up to the frame update events
    SubscribeToEvents();

    InitControls();

    // Set the mouse mode to use in the sample
    Sample::InitMouseMode(MM_FREE);
}
void mainNajwa::CreateDraggableFish()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();

    // Create a draggable Fish button
    Button* draggableFish = new Button(context_);
    draggableFish->SetTexture(cache->GetResource<Texture2D>("Textures/ball.png")); // Set texture
    draggableFish->SetBlendMode(BLEND_ADD);
    draggableFish->SetSize(128, 128);
    draggableFish->SetPosition((graphics->GetWidth() - draggableFish->GetWidth()) / 2, 200);
    draggableFish->SetName("Fish");
    ui->GetRoot()->AddChild(draggableFish);

    // Add a tooltip to Fish button
    ToolTip* toolTip = new ToolTip(context_);
    draggableFish->AddChild(toolTip);
    toolTip->SetPosition(IntVector2(draggableFish->GetWidth() + 5, draggableFish->GetWidth() / 2)); // slightly offset from close button
    BorderImage* textHolder = new BorderImage(context_);
    toolTip->AddChild(textHolder);
    textHolder->SetStyle("ToolTipBorderImage");


    // Subscribe draggableFish to Drag Events (in order to make it draggable)
    // See "Event list" in documentation's Main Page for reference on available Events and their eventData
    SubscribeToEvent(draggableFish, E_DRAGBEGIN, URHO3D_HANDLER(mainNajwa, HandleDragBegin));
    SubscribeToEvent(draggableFish, E_DRAGMOVE, URHO3D_HANDLER(mainNajwa, HandleDragMove));
    SubscribeToEvent(draggableFish, E_DRAGEND, URHO3D_HANDLER(mainNajwa, HandleDragEnd));
}
void mainNajwa::HandleDragBegin(StringHash eventType, VariantMap& eventData)
{
    startTime = std::clock(); //Start timer

    // Get UIElement relative position where input (touch or click) occurred (top-left = IntVector2(0,0))
    dragBeginPosition_ = IntVector2(eventData["ElementX"].GetInt(), eventData["ElementY"].GetInt());
    BeginPosition_ = IntVector2(eventData["X"].GetInt(), eventData["Y"].GetInt());

}


void mainNajwa::HandleDragMove(StringHash eventType, VariantMap& eventData)
{
    IntVector2 dragCurrentPosition = IntVector2(eventData["X"].GetInt(), eventData["Y"].GetInt());
    UIElement* draggedElement = static_cast<UIElement*>(eventData["Element"].GetPtr());
    draggedElement->SetPosition(dragCurrentPosition - dragBeginPosition_);
    draggedElement->SetSize(dragCurrentPosition.y_/4, dragCurrentPosition.y_/4);
}

void mainNajwa::HandleDragEnd(StringHash eventType, VariantMap& eventData) // For reference (not used here)
{
    IntVector2 dragCurrentPosition = IntVector2(eventData["X"].GetInt(), eventData["Y"].GetInt());
    std::cout << "BEGIN Position X=" <<  BeginPosition_.x_ << " Y=" <<  BeginPosition_.y_ << std::endl;
    std::cout << "END Position X=" <<  dragCurrentPosition.x_ << " Y=" <<  dragCurrentPosition.y_ << std::endl;
    double distance = sqrt((BeginPosition_.x_  - dragCurrentPosition.x_)^2 + (BeginPosition_.y_  - dragCurrentPosition.y_)^2);
    // Calculate the power based on the time and the distance
    clock_t endTime = std::clock();
    double timePassed = (endTime - startTime) / ((double)CLOCKS_PER_SEC/10);
    std::cout << "Time passed : " << timePassed << std::endl;
    double speed = distance/timePassed;


   // UI* ui = GetSubsystem<UI>();
   // ui->GetRoot()->RemoveChild(mainNajwa_[0]);

    /*  int position = 0;
      IntVector2 moving =  IntVector2(528 + position,256 + position);
      UIElement* draggedElement = static_cast<UIElement*>(eventData["Element"].GetPtr());

      for (int i=0;i<10;i++){
          moving.x_= moving.x_ + position;
          draggedElement->SetPosition(moving);
          draggedElement->SetSize(moving.y_/4, moving.y_/4);
          std::cout << "moving= " << moving.x_ << std::endl;
          position = 10;
          sleep(2);
      }*/


}
void mainNajwa::InitControls() {
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
    button->SetName("Button");
    button->SetMinHeight(24);

    // Set the text displayed on the button
    Font* font = cache->GetResource<Font>("Fonts/Anonymous Pro.ttf");
    Text* buttonText = button->CreateChild<Text>();
    buttonText->SetFont(font, 12);
    buttonText->SetAlignment(HA_CENTER, VA_CENTER);
    buttonText->SetText("PLAY");

    // Add controls to Window
    window_->AddChild(button);

    // Apply previously set default style
    button->SetStyleAuto();
    SubscribeToEvent(button, E_RELEASED, URHO3D_HANDLER(mainNajwa,HandlePlayPressed));

}

void mainNajwa::HandlePlayPressed(StringHash eventType, VariantMap& eventData)
{
    UI* ui = GetSubsystem<UI>();

    ui->GetRoot()->RemoveAllChildren();

    // Create the mainNajwa to the user interface
    CreatemainNajwa();
    CreateDraggableFish();

}
void mainNajwa::CreatemainNajwa()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();

    // Get rendering window size as floats
    float width = (float)graphics->GetWidth();
    float height = (float)graphics->GetHeight();



    // Get the Urho3D fish texture
    //   Texture2D* decalTex = cache->GetResource<Texture2D>("Textures/UrhoDecal.dds");
    Texture2D* decalTex = cache->GetResource<Texture2D>("Textures/beer.png");

    for (unsigned i = 0; i < NUM_mainNajwa; ++i)
    {
        // Create a new sprite, set it to use the texture
        SharedPtr<Sprite> sprite(new Sprite(context_));
        sprite->SetTexture(decalTex);

        // The UI root element is as big as the rendering window, set random position within it
        //   sprite->SetPosition(Vector2(Random() * width, Random() * height));
        sprite->SetPosition(Vector2((width+i*100)/2,(height+i*100)/2));

        // Set sprite size & hotspot in its center
        sprite->SetSize(IntVector2(128, 128));
        sprite->SetHotSpot(IntVector2(64, 64));

        // Set random color and additive blending mode
        sprite->SetColor(Color(Random(0.5f) + 0.5f, Random(0.5f) + 0.5f, Random(0.5f) + 0.5f));
        sprite->SetBlendMode(BLEND_ADD);

        // Add as a child of the root UI element
        ui->GetRoot()->AddChild(sprite);
        std::cout << "Position of cup " << i << " : " << ui->GetRoot()->FindChild(sprite) << std::endl;

        // Store sprite's velocity as a custom variable
        sprite->SetVar(VAR_VELOCITY, Vector2(Random(200.0f) - 100.0f, Random(200.0f) - 100.0f));

        // Store mainNajwa to our own container for easy movement update iteration
        mainNajwa_.Push(sprite);

    }

}


void mainNajwa::SubscribeToEvents()
{
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(mainNajwa, HandleUpdate));
}

void mainNajwa::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Move mainNajwa, scale movement with time step
    // MovemainNajwa(timeStep);
}
