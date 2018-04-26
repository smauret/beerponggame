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
static const unsigned NUM_main = 6;

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
    InitWelcomePage();

    // Set the mouse mode to use in the sample
    Sample::InitMouseMode(MM_FREE);
}
void main::CreatedraggableBall()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();

    // Create a draggable Ball button
    SharedPtr<Button> draggableBall (new Button(context_));
    draggableBall->SetTexture(cache->GetResource<Texture2D>("Textures/ball.png")); // Set texture
    draggableBall->SetBlendMode(BLEND_ALPHA);
    draggableBall->SetSize(68, 68);
    draggableBall->SetPosition(3*(graphics->GetWidth() - draggableBall->GetWidth()) / 4, 200);
    draggableBall->SetName("Ball");
    ui->GetRoot()->AddChild(draggableBall);

    // Add a tooltip to Ball button
    ToolTip* toolTip = new ToolTip(context_);
    draggableBall->AddChild(toolTip);
    draggedElement_ = draggableBall;
    toolTip->SetPosition(IntVector2(draggableBall->GetWidth() + 5, draggableBall->GetWidth() / 2)); // slightly offset from close button

    SubscribeToEvent(E_MOUSEBUTTONDOWN, URHO3D_HANDLER(main,HandleMouse));

    uielem_.Push(draggableBall);

}
void main::HandleMouse(StringHash eventType, VariantMap& eventData)
{
    int x = GetSubsystem<Input>()->GetMousePosition().x_;
    int y = GetSubsystem<Input>()->GetMousePosition().y_;
    draggedElement_->SetSize(68,68);
    draggedElement_->SetPosition(x - (draggedElement_->GetSize().x_)/2,y - (draggedElement_->GetSize().y_)/2);

    // Subscribe draggableBall to Drag Events (in order to make it draggable)
    // See "Event list" in documentation's Main Page for reference on available Events and their eventData
    SubscribeToEvent(draggedElement_, E_DRAGBEGIN, URHO3D_HANDLER(main, HandleDragBegin));
    SubscribeToEvent(draggedElement_, E_DRAGMOVE, URHO3D_HANDLER(main, HandleDragMove));
    SubscribeToEvent(draggedElement_, E_DRAGEND, URHO3D_HANDLER(main, HandleDragEnd));
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

    draggedElement_ = static_cast<UIElement*>(eventData["Element"].GetPtr());
    draggedElement_ ->SetPosition(dragCurrentPosition - dragBeginPosition_);
    draggedElement_ ->SetSize(std::min(dragCurrentPosition.y_/4,68), std::min(dragCurrentPosition.y_/4,68));
}

void main::HandleDragEnd(StringHash eventType, VariantMap& eventData)
{
    IntVector2 dragCurrentPosition = IntVector2(eventData["X"].GetInt(), eventData["Y"].GetInt());
    // Calculate the power based on the time and the distance
    double speed = GetSpeed(BeginPosition_,dragCurrentPosition);
    std::cout << "Speed : " << speed << std::endl;
    if(speed > 100 && speed < 1500){
        double restSpeed = speed /10;
        speed = 350+restSpeed;
    }
    std::cout << "Speed : " << speed << std::endl;
    // Calculate the rotation angle
    double rotation_angle = GetRotation(BeginPosition_,dragCurrentPosition);
    std::cout << "Rotation angle in degrees : " << rotation_angle* 180.0 / M_PI << std::endl;
    if(rotation_angle<0.7848){
        rotation_angle=0.7848;
    }else if(rotation_angle>2.3544){
        rotation_angle=2.3544;
    }else{
        double rest = (rotation_angle - 0.7848)/9;
        rotation_angle = 1.4388 + rest;
    }
    // Calculate trajectory
    IntVector3 finalPositionCm = GetInitPosCm(dragCurrentPosition);
    cupScored = -1;
    //vector<Vec3<int>> ballTrajectory = lucas_.throwBall(M_PI/4, rotation_angle, (double)(finalPositionCm.z_), speed*100, (double)(finalPositionCm.x_), 0, cupScored);
    vector<Vec3<int>> ballTrajectory = lucas_.throwBall(M_PI/4, rotation_angle, 50, speed, (double)(finalPositionCm.x_), 0, cupScored);
    //vector<Vec3<int>> ballTrajectory = lucas_.throwBall (static_cast<double>(M_PI / 4), static_cast<double>(M_PI / 2), 100, 405, 30, 0, cupScored);
    //ThrowResult(cupScored);
    std::cout << "Cup scored " << cupScored << std::endl << std::endl;
    for (int i=0; i<ballTrajectory.size(); i++) {
        graphicsTrajectory_.emplace_back(1024/2, 768/2,0);
    }
    lucas_.get_xzSize_graphics(ballTrajectory, graphicsTrajectory_);
    //lucas_.get_z_graphics(ballTrajectory, graphicsTrajectory_);
    //lucas_.get_x_graphics(ballTrajectory, graphicsTrajectory_);
    //ui->GetRoot()->RemoveChild(main_[0]);;
    k=0;

    UnsubscribeFromEvent(E_DRAGBEGIN);
    UnsubscribeFromEvent(E_DRAGMOVE);
    UnsubscribeFromEvent(E_DRAGEND);
    UnsubscribeFromEvent(E_MOUSEBUTTONDOWN);

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(main, HandleUpdate));
}

void main::ThrowResult(int cupScored){
    UI* ui = GetSubsystem<UI>();
  //  SharedPtr<Text> throwResult(new Text(context_));
    if(cupScored == -1){
        ui->GetRoot()->RemoveChild(uielem_[4]);
        SharedPtr<Text> textUpdate(new Text(context_));
        textUpdate->SetText("Welcome to the beer pong game \nFailed = +0 Point");
        textUpdate->SetStyleAuto();
        textUpdate->SetOpacity(1.0);
        textUpdate->SetFont("Fonts/Anonymous Pro.ttf",30);
        textUpdate->SetPosition(0,300);
        textUpdate->SetBringToBack(true);
        uielem_[4]->RemoveAllChildren();
        uielem_[4]->AddChild(textUpdate);
        ui->GetRoot()->AddChild(uielem_[4]);

    }else{

        ui->GetRoot()->RemoveChild(uielem_[4]);

        SharedPtr<Text> textUpdate(new Text(context_));
        textUpdate->SetText("Welcome to the beer pong game \nSuccess = +1 Point !  ");

        textUpdate->SetStyleAuto();
        textUpdate->SetOpacity(1.0);
        textUpdate->SetFont("Fonts/Anonymous Pro.ttf",30);
        textUpdate->SetPosition(0,300);
        textUpdate->SetBringToBack(true);
        uielem_[4]->RemoveAllChildren();
        uielem_[4]->AddChild(textUpdate);
        ui->GetRoot()->AddChild(uielem_[4]);
        ui->GetRoot()->RemoveChild(main_[cupScored]);
    }
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
    cout << "lancé: x : " << initPosCm.x_ << " | y : " << initPosCm.y_ << " | z : " << initPosCm.z_ << endl;
    return initPosCm;
}

void main::InitWelcomePage() {
    UI* ui = GetSubsystem<UI>();
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
    // Create the Window and add it to the UI's root node
    SharedPtr<Window> window_(new Window(context_));
    ui->GetRoot()->AddChild(window_);
    // Set the loaded style as default style
    ui->GetRoot()->SetDefaultStyle(style);

    Graphics* graphics = GetSubsystem<Graphics>();
    // Get rendering window size as floats
    //graphics->ToggleFullscreen();
    bool verif = graphics->SetMode(1024,768,0,0,0,0,0,0,0,0,60);
    float width = (float)graphics->GetWidth();
    float height = (float)graphics->GetHeight();
    cout << "verif: " << verif << " width : " << (float)graphics->GetWidth() << " & height " << (float)graphics->GetHeight();

    // Set Window size and layout settings
    window_->SetMinWidth(384);
    window_->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
    window_->SetAlignment(HA_CENTER, VA_CENTER);
    window_->SetName("Window");
    window_->SetStyleAuto();
    uielem_.Push(window_);

    // Create a Button
    SharedPtr<Button> button(new Button(context_));
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
    uielem_.Push(button);


    // Display background image
    Texture2D* background = cache->GetResource<Texture2D>("Textures/background_beer.jpg");
    SharedPtr<BorderImage> back(new BorderImage(context_));
    ui->GetRoot()->AddChild(back);
    back->SetTexture(background);
    back->SetSize(width,height);
    back->SetBringToBack(true);
    back->SetOpacity(0.3);
    uielem_.Push(back);

    // Title of the first page
    SharedPtr<Text> title(new Text(context_));
    title->SetText("Welcome to the BeerPong Game !");
    ui->GetRoot()->AddChild(title);
    title->SetStyleAuto();
    title->SetOpacity(1.0);
    title->SetFont("Fonts/Anonymous Pro.ttf",30);
    title->SetPosition(((int)width-title->GetSize().x_)/2,(int)height/3);
    uielem_.Push(title);

    SubscribeToEvent(button, E_RELEASED, URHO3D_HANDLER(main,HandlePlayPressed));
}

void main::HandleReturnPressed(StringHash eventType, VariantMap& eventData)
{
    UI* ui = GetSubsystem<UI>();
    // Remove play screen
    ui->GetRoot()->RemoveAllChildren();
    //Add Button in Window
    uielem_[0]->AddChild(uielem_[1]);
    // Add Window
    ui->GetRoot()->AddChild(uielem_[0]);
    // Add Background image
    ui->GetRoot()->AddChild(uielem_[2]);
    // Add Title
    ui->GetRoot()->AddChild(uielem_[3]);
}

void main::HandlePlayPressed(StringHash eventType, VariantMap& eventData)
{
    //Player[2] players;
    // Create player
    lucas_ = Player("Lucas",6);
    // Graphics
    UI* ui = GetSubsystem<UI>();
    ui->GetRoot()->RemoveAllChildren();
    InitBoardGame();
    CreatedraggableBall();
}

void main::InitBoardGame()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Graphics* graphics = GetSubsystem<Graphics>();
    UI* ui = GetSubsystem<UI>();

    // Get rendering window size as floats
    float width = (float)graphics->GetWidth();
    float height = (float)graphics->GetHeight();

    // TODO ajouter l'image de background
    // Display Background picture
   /* Texture2D* backTex = cache->GetResource<Texture2D>("Textures/background_beer.jpg");
    SharedPtr<BorderImage> backBoard(new BorderImage(context_));
    ui->GetRoot()->AddChild(backBoard);
    backBoard->SetTexture(backTex);
    backBoard->SetSize(width,height);
    backBoard->SetBringToBack(true);*/
   // backBoard->SetBlendMode(BLEND_ADD);

    //backBoard->SetPosition(0,317);

    // Display table image
    Texture2D* tableTex = cache->GetResource<Texture2D>("Textures/Table.png");
    SharedPtr<BorderImage> table(new BorderImage(context_));
    ui->GetRoot()->AddChild(table);
    table->SetTexture(tableTex);
    table->SetSize(width,height/2);
    table->SetBringToBack(true);
    table->SetPosition(0,317);
  //  table->SetBlendMode(BLEND_ALPHA);

    // TODO ajouter table à uielem_
  //  uielem_.Push(back);

    // Get the cup texture
    Texture2D* decalTex = cache->GetResource<Texture2D>("Textures/back_beer.png");
    vector<Vec2i> positionCups;
    positionCups.emplace_back(432 + 38 , 267 + 50);
    positionCups.emplace_back(513 + 38 , 267 + 50);
    positionCups.emplace_back(594 + 38 , 267 + 50);
    positionCups.emplace_back(475 + 38 , 292 + 50);
    positionCups.emplace_back(553 + 38 , 292 + 50);
    positionCups.emplace_back(512 + 38 , 325 + 50);

    for (unsigned i = 0; i < NUM_main; ++i)
    {
        // Create a new sprite, set it to use the texture
        SharedPtr<Sprite> sprite(new Sprite(context_));

        sprite->SetTexture(decalTex);

        // Set position of the cup
       // sprite->SetPosition(Vector2((width+i*100)/2,(height+i*100)/2));
        sprite->SetPosition(positionCups[i].x, positionCups[i].y);

        // Set sprite size & hotspot in its center
        sprite->SetSize(IntVector2(76, 100));
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


    SharedPtr<Window> textWindow(new Window(context_));
    // Set Window size and layout settings
    // TODO : Adapter la taille de la window au contenu ?

    textWindow->SetMaxWidth(250);
    textWindow->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
    textWindow->SetPosition(0, 200);
    textWindow->SetName("textWindow");
    textWindow->SetStyleAuto();

    SharedPtr<Text> title1(new Text(context_));
    title1->SetText("Welcome to the BeerPong Game !");
    title1->SetStyleAuto();
    title1->SetOpacity(1.0);
    title1->SetFont("Fonts/Anonymous Pro.ttf",30);
    title1->SetPosition(0,100);
    title1->SetBringToBack(true);
    uielem_.Push(textWindow);
    textWindow->AddChild(title1);
    ui->GetRoot()->AddChild(textWindow);

    CreateReturnButton();
}

void main::CreateReturnButton(){
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();

    //Create return button
    XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
    // Create the Window and add it to the UI's root node
    SharedPtr<Window> windowReturn_(new Window(context_));
    ui->GetRoot()->AddChild(windowReturn_);

    // Set the loaded style as default style
    ui->GetRoot()->SetDefaultStyle(style);

    // Set Window size and layout settings
    windowReturn_->SetMinWidth(100);
    windowReturn_->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
    //  window_->SetAlignment(HA_CENTER, VA_CENTER);
    windowReturn_->SetAlignment(HA_LEFT, VA_TOP);
    windowReturn_->SetName("Window");
    windowReturn_->SetStyleAuto();
    uielem_.Push(windowReturn_);

    // Create a Button
    SharedPtr<Button> buttonReturn (new Button(context_));
    // Add controls to Window
    windowReturn_->AddChild(buttonReturn);
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
    SubscribeToEvent(buttonReturn, E_RELEASED, URHO3D_HANDLER(main,HandleReturnPressed));
    uielem_.Push(buttonReturn);
}

void main::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    if(k<graphicsTrajectory_.size()){
        draggedElement_->SetPosition(graphicsTrajectory_[k].getX(), graphicsTrajectory_[k].getZ());
        draggedElement_->SetSize(graphicsTrajectory_[k].getY(),graphicsTrajectory_[k].getY());
        k=k+1;
    } else {
        //draggedElement_->SetPosition(1024/2, 768/2);
        //draggedElement_->SetSize(68,68);
        graphicsTrajectory_.clear();
        ThrowResult(cupScored);
        UnsubscribeFromEvent(E_UPDATE);
        SubscribeToEvent(E_MOUSEBUTTONDOWN, URHO3D_HANDLER(main,HandleMouse));
    }
}
