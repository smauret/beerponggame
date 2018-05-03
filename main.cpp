//ECE-6122: Advanced Programming Techniques
//Beer Pong Game
//Najwa Harif, Sarah Mauret, Hugo Blech, Lucas Chaulan, Elliot Hallais

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <unistd.h>

#include "main.h"


#include <Urho3D/DebugNew.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/ToolTip.h>
#include <Urho3D/UI/Text.h>


#include <iostream>
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
    if(uielem_.Size() < 12) {
        // Create a draggable Ball button
        SharedPtr<Button> draggableBall(new Button(context_));
        draggableBall->SetTexture(cache->GetResource<Texture2D>("Textures/ball.png")); // Set texture
        draggableBall->SetBlendMode(BLEND_ALPHA);
        draggableBall->SetSize(68, 68);
        draggableBall->SetPosition(3 * (graphics->GetWidth() - draggableBall->GetWidth()) / 4, 200);
        draggableBall->SetName("Ball");

        draggableBall->SetPriority(410);

        ui->GetRoot()->AddChild(draggableBall);
        draggedElement_ = draggableBall;
        uielem_.Push(draggableBall);
    }else{
        cout << "   Adding ball again" << endl;
        ui->GetRoot()->AddChild(uielem_[11]);
    }
    SubscribeToEvent(E_MOUSEBUTTONDOWN, URHO3D_HANDLER(main,HandleMouse));
    //cout << "Create draggable ball" << "  uielem_ size : " << uielem_.Size() << endl;
}

void main::HandleMouse(StringHash eventType, VariantMap& eventData)
{
    UI* ui = GetSubsystem<UI>();
    ui->GetRoot()->RemoveChild(uielem_[7]);
    Color *c = new Color(0.25, 0.25, 0.25, 1.0);

    SharedPtr<Text> playerName(new Text(context_));
    string playerNameString = currentPlayer_->getName();
    playerName->SetText(playerNameString.c_str());
    playerName->SetTextAlignment(HA_CENTER);
    playerName->SetHorizontalAlignment(HA_CENTER);
    playerName->SetFont("Fonts/Roboto-Bold.ttf", 18);
    playerName->SetColor(*c);
    uielem_[7]->RemoveAllChildren();
    uielem_[7]->AddChild(playerName);

    SharedPtr<Text> totalThrows(new Text(context_));
    string totalThrowsString;

    if (currentPlayer_->getTotalThrows() == 0)
        totalThrowsString = "0 throw";
    else if (currentPlayer_->getTotalThrows() == 1)
        totalThrowsString = std::to_string(currentPlayer_->getTotalThrows()) + " throw";
    else
        totalThrowsString = std::to_string(currentPlayer_->getTotalThrows()) + " throws";

    totalThrows->SetText(totalThrowsString.c_str());
    totalThrows->SetFont("Fonts/Roboto-Bold.ttf",14);
    totalThrows->SetColor(*c);
    //totalThrows->SetPosition(25, 45);
    totalThrows->SetTextAlignment(HA_CENTER);
    totalThrows->SetHorizontalAlignment(HA_CENTER);
    uielem_[7]->AddChild(totalThrows);

    ui->GetRoot()->AddChild(uielem_[7]);
    int x = GetSubsystem<Input>()->GetMousePosition().x_;
    int y = GetSubsystem<Input>()->GetMousePosition().y_;
    draggedElement_->SetSize(68,68);
    draggedElement_->SetPosition(x - (draggedElement_->GetSize().x_)/2,y - (draggedElement_->GetSize().y_)/2);
    draggedElement_->SetPriority(410);

/*    if (*currentPlayer_ == computer_)
        ballTrajectory_ = currentPlayer_->throwBall(cupScored);*/

    //cout << "Displaying cups of " << currentPlayer_->getName() << endl;
    DisplayCups(*currentPlayer_);
    // Subscribe draggableBall to Drag Events (in order to make it draggable)
    // See "Event list" in documentation's Main Page for reference on available Events and their eventData
    cout << "Current player : " << currentPlayer_->getName() << endl;
    if (*currentPlayer_ == computer_ && count == 0){
        count++;
        ballTrajectory_ = currentPlayer_->throwBall(cupScored);
        currentPlayer_->get_xzSize_graphics(ballTrajectory_, graphicsTrajectory_, cupScored);
        k=0;
        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(main, HandleUpdate));
    }
    cout << "count : " << count << endl;

    SubscribeToEvent(draggedElement_, E_DRAGBEGIN, URHO3D_HANDLER(main, HandleDragBegin));
    SubscribeToEvent(draggedElement_, E_DRAGMOVE, URHO3D_HANDLER(main, HandleDragMove));
    SubscribeToEvent(draggedElement_, E_DRAGEND, URHO3D_HANDLER(main, HandleDragEnd));

/*    cout << "Current player : " << currentPlayer_->getName() << endl;
    cout << "Sarah cup on table : " << sarah_.getCup(1).isOnTable() << endl;
    cout << "Lucas cup on table : " << lucas_.getCup(1).isOnTable() << endl << endl;*/
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
    /*if(speed > 100 && speed < 1500){
        double restSpeed = speed /10;
        speed = 350+restSpeed;
    }*/
    speed = 350 + (speed - 350) / 10;


    // Calculate the rotation angle
    double rotation_angle = GetRotation(BeginPosition_,dragCurrentPosition);

    rotation_angle = M_PI/2 + (rotation_angle - M_PI/2) / 8;

    // Calculate trajectory
    IntVector3 finalPositionCm = GetInitPosCm(dragCurrentPosition);
    cupScored = -1;
    if(playMode_ == 0 || playMode_ == 1){
        ballTrajectory_ = currentPlayer_->throwBall(M_PI/4, rotation_angle, 50, speed, (double)(finalPositionCm.x_), 0, cupScored);
    }else if (playMode_ == 2){
/*        if (*currentPlayer_ == computer_)
            //ballTrajectory_ = currentPlayer_->throwBall(cupScored);*/
        if (! (*currentPlayer_ == computer_))
            ballTrajectory_ = currentPlayer_->throwBall(M_PI/4, rotation_angle, 50, speed, (double)(finalPositionCm.x_), 0, cupScored);
    }

    currentPlayer_->get_xzSize_graphics(ballTrajectory_, graphicsTrajectory_, cupScored);
    k=0;
    UnsubscribeFromEvent(E_DRAGBEGIN);
    UnsubscribeFromEvent(E_DRAGMOVE);
    UnsubscribeFromEvent(E_DRAGEND);
    UnsubscribeFromEvent(E_MOUSEBUTTONDOWN);

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(main, HandleUpdate));
}

void main::ThrowResult(int cupScored)
{
    UI* ui = GetSubsystem<UI>();

    if(cupScored != -1){
        ui->GetRoot()->RemoveChild(uielem_[11]);
        ui->GetRoot()->AddChild(splash_[cupScored]);
        currentPlayer_->getCup(cupScored).setOnTable(false);
        currentPlayer_->setCupsLeft(currentPlayer_->getCupsLeft()-1);
    }
    //cout << currentPlayer_->getName() << "  cups left " << currentPlayer_->getCupsLeft() << " Sarah : " << sarah_.getCupsLeft() << "  Lucas : " << lucas_.getCupsLeft() << endl;

    if (currentPlayer_->getCupsLeft() == 0 and !AnyWinner())
    {
        SharedPtr<Window> textWindow(new Window(context_));
        // Set Window size and layout settings
        textWindow->SetFixedSize(1024,75);
        textWindow->SetPosition(0, 145);
        textWindow->SetName("winWindow");
        textWindow->SetStyle("WindowCustWin");
        textWindow->SetPriority(1000);
        textWindow->SetOpacity(0.8);
        Color *c = new Color(0.25, 0.25, 0.25, 1.0);

        SharedPtr<Text> win (new Text(context_));
        string welcome = "Congratulations " + currentPlayer_->getName() + ", you have won the game in " + std::to_string(currentPlayer_->getTotalThrows()) + " throws!";
        win->SetText(welcome.c_str());
        win->SetAlignment(HA_CENTER, VA_CENTER);
        win->SetFont("Fonts/Roboto-Bold.ttf", 18);
        win->SetColor(*c);
        textWindow->AddChild(win);
        ui->GetRoot()->AddChild(textWindow);
        currentPlayer_->win();
    }

    if (playMode_ == 1){
        if(currentPlayer_ == &sarah_)
            currentPlayer_ = &lucas_;
        else
            currentPlayer_ = &sarah_;
    }else if (playMode_ == 2){
        if(currentPlayer_ == &sarah_)
            currentPlayer_ = &computer_;
        else
            currentPlayer_ = &sarah_;
    }else{
        //Do nothing, keep only one player
    }
}

// Function to translate a point in graphics (pixels) to a point on the table (cm)
// Assumption : 90° view
IntVector3 main::GetInitPosCm(IntVector2 initPos)
{
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
    //cout << "lancé: x : " << initPosCm.x_ << " | y : " << initPosCm.y_ << " | z : " << initPosCm.z_ << endl;
    return initPosCm;
}

void main::InitWelcomePage()
{
    UI* ui = GetSubsystem<UI>();
    ui->GetRoot()->SetSortChildren(true);
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
    // Set the loaded style as default style
    ui->GetRoot()->SetDefaultStyle(style);

    Graphics* graphics = GetSubsystem<Graphics>();
    // Get rendering window size as floats
    //graphics->ToggleFullscreen();
    bool verif = graphics->SetMode(1024,768,0,0,0,0,0,0,0,0,60);
    float width = (float)graphics->GetWidth();
    float height = (float)graphics->GetHeight();
    //cout << "verif: " << verif << " width : " << (float)graphics->GetWidth() << " & height " << (float)graphics->GetHeight();

    // Display background image
    Texture2D* background = cache->GetResource<Texture2D>("Textures/home_background.png");
    SharedPtr<BorderImage> back(new BorderImage(context_));
    ui->GetRoot()->AddChild(back);
    back->SetTexture(background);
    back->SetSize(width,height);
    back->SetBringToBack(true);
    back->SetOpacity(1);
    back->SetPriority(10);
    uielem_.Push(back);

    Color *c = new Color(0.25, 0.25, 0.25, 1.0);
    Font* font = cache->GetResource<Font>("Fonts/Roboto-Bold.ttf");
    // Create 3 Buttons
    // Button 1 - Solo
    SharedPtr<Button> button(new Button(context_));
    ui->GetRoot()->AddChild(button);
    button->SetName("Practice");
    button->SetMinHeight(53);
    button->SetMinWidth(234);
    button->SetPosition(399,548);
    // Set the text displayed on the button
    Text* buttonText = button->CreateChild<Text>();
    buttonText->SetFont(font, 24);
    buttonText->SetAlignment(HA_CENTER, VA_CENTER);
    buttonText->SetText("Practice");
    buttonText->SetColor(*c);
    // Apply custom style we created in xml
    button->SetStyle("ButtonCust");
    button->SetPriority(100);
    uielem_.Push(button);

    //Button 2 - 1vs1
    SharedPtr<Button> button2(new Button(context_));
    ui->GetRoot()->AddChild(button2);
    button2->SetName("ButtonMulti");
    button2->SetMinHeight(53);
    button2->SetMinWidth(234);
    button2->SetPosition(399,603);
    // Set the text displayed on the button
    Text* button2Text = button2->CreateChild<Text>();
    button2Text->SetFont(font, 24);
    button2Text->SetAlignment(HA_CENTER, VA_CENTER);
    button2Text->SetText("Multi");
    button2Text->SetColor(*c);
    // Apply custom style we created in xml
    button2->SetStyle("ButtonCust");
    button2->SetPriority(100);
    uielem_.Push(button2);

    // Button 3 - 1vsPC
    SharedPtr<Button> button3(new Button(context_));
    ui->GetRoot()->AddChild(button3);
    button3->SetName("ButtonComputer");
    button3->SetMinHeight(53);
    button3->SetMinWidth(234);
    button3->SetPosition(399,658);
    // Set the text displayed on the button
    Text* button3Text = button3->CreateChild<Text>();
    button3Text->SetFont(font, 24);
    button3Text->SetAlignment(HA_CENTER, VA_CENTER);
    button3Text->SetText("Computer");
    button3Text->SetColor(*c);
    // Apply custom style we created in xml
    button3->SetStyle("ButtonCust");
    button3->SetPriority(100);
    uielem_.Push(button3);

    // Title of the first page
    SharedPtr<Text> title(new Text(context_));
    title->SetText("Welcome to the BeerPong Game !");
    ui->GetRoot()->AddChild(title);
    title->SetStyleAuto();
    title->SetOpacity(1.0);
    title->SetFont("Fonts/Roboto-Bold.ttf",30);
    title->SetPosition(((int)width-title->GetSize().x_)/2,1*(int)height/5);
    title->SetPriority(10);
    uielem_.Push(title);
    //cout << "Init Welcome Page" << "  size uielem_ : " << uielem_.Size() << endl;

    SubscribeToEvent(button, E_RELEASED, URHO3D_HANDLER(main,HandlePlayPressed));
    SubscribeToEvent(button2, E_RELEASED, URHO3D_HANDLER(main,HandlePlayPressed));
    SubscribeToEvent(button3, E_RELEASED, URHO3D_HANDLER(main,HandlePlayPressed));
}

void main::HandleReturnPressed(StringHash eventType, VariantMap& eventData)
{
    cout << "Handle press return" << "   uielem_ size : " << uielem_.Size() << endl;
    cupScored = -1;
    UI* ui = GetSubsystem<UI>();
    // Remove play screen
    ui->GetRoot()->RemoveAllChildren();
    //Add Background image
    ui->GetRoot()->AddChild(uielem_[0]);
    // Add Buttons
    ui->GetRoot()->AddChild(uielem_[1]);
    ui->GetRoot()->AddChild(uielem_[2]);
    ui->GetRoot()->AddChild(uielem_[3]);
    // Add Title
    ui->GetRoot()->AddChild(uielem_[4]);
}

void main::HandlePlayPressed(StringHash eventType, VariantMap& eventData)
{

    //Player[2] players;
    auto* clicked = static_cast<UIElement*>(eventData[UIMouseClick::P_ELEMENT].GetPtr());
    string buttonName = clicked->GetName().CString();
    // Create player
    lucas_ = Player("Lucas",6);
    sarah_ = Player("Sarah",6);
    computer_ = Player("PC",6);
    // Graphics

    if(buttonName.compare("Practice")==0)
        playMode_ = 0;
    else if (buttonName.compare("ButtonMulti")==0)
        playMode_ = 1;
    else
        playMode_ = 2;

    UI* ui = GetSubsystem<UI>();
    ui->GetRoot()->RemoveAllChildren();
    ui->GetRoot()->SetSortChildren(true);
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

    // Display background image
    Texture2D* background = cache->GetResource<Texture2D>("Textures/bg.png");
    SharedPtr<BorderImage> back(new BorderImage(context_));
    ui->GetRoot()->AddChild(back);
    back->SetTexture(background);
    back->SetSize(width,height);
    back->SetBringToBack(true);
    back->SetOpacity(1);
    back->SetPriority(10);
    uielem_.Push(back);

    Texture2D* background2 = cache->GetResource<Texture2D>("Textures/bg_bar.png");
    SharedPtr<BorderImage> back2(new BorderImage(context_));
    back2->SetTexture(background2);
    back2->SetSize(width,height);
    back2->SetBringToBack(true);
    back2->SetOpacity(1);
    back2->SetPriority(10);
    bg_.Push(back2);

    // Display table image
    if(uielem_.Size() < 12){
        Texture2D* tableTex = cache->GetResource<Texture2D>("Textures/Table.png");
        SharedPtr<BorderImage> table(new BorderImage(context_));
        table->SetTexture(tableTex);
        table->SetSize(889,461);
        table->SetBringToBack(true);
        table->SetPosition(70,308);
        ui->GetRoot()->AddChild(table);
        table->SetPriority(110);
        uielem_.Push(table);
    }else
    {
        ui->GetRoot()->AddChild(uielem_[6]);
    }

    currentPlayer_ = &sarah_;
    DisplayCups(*currentPlayer_);


    //Window score in boardgame
    if(uielem_.Size() < 12) {
        SharedPtr<Window> textWindow(new Window(context_));
        // Set Window size and layout settings
        textWindow->SetFixedSize(118,80);
        textWindow->SetPosition(900, 5);
        textWindow->SetName("textWindow");
        textWindow->SetStyle("WindowCust");
        textWindow->SetPriority(300);
        textWindow->SetLayout(LM_VERTICAL);
        textWindow->SetLayoutSpacing(6);
        uielem_.Push(textWindow);
        Color *c = new Color(0.25, 0.25, 0.25, 1.0);

        SharedPtr<Text> title1(new Text(context_));
        string welcome = currentPlayer_->getName();
        title1->SetText(welcome.c_str());
        title1->SetHorizontalAlignment(HA_CENTER);
        title1->SetTextAlignment(HA_CENTER);
        title1->SetFont("Fonts/Roboto-Bold.ttf", 18);
        title1->SetColor(*c);
        textWindow->AddChild(title1);
        uielem_.Push(title1);

        SharedPtr<Text> title2(new Text(context_));
        string welcome2 = "0 throw";
        title2->SetText(welcome2.c_str());
        title2->SetHorizontalAlignment(HA_CENTER);
        title2->SetTextAlignment(HA_CENTER);
        title2->SetFont("Fonts/Roboto-Bold.ttf", 14);
        title2->SetColor(*c);
        textWindow->AddChild(title2);
        uielem_.Push(title2);

        textWindow->SetPriority(310);
        ui->GetRoot()->AddChild(textWindow);
    }else{
        uielem_[7]->RemoveAllChildren();
        uielem_[7]->AddChild(uielem_[8]);
        uielem_[7]->AddChild(uielem_[9]);
        ui->GetRoot()->AddChild(uielem_[7]);
    }
    CreateReturnButton();
}

void main::DisplayCups(Player player)
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();
    if(main_.Size() == 0) {
        Texture2D *decalTex = cache->GetResource<Texture2D>("Textures/back_beer_red.png");
        Texture2D *splashTex = cache->GetResource<Texture2D>("Textures/beer_splash.png");
        Texture2D *blueTex = cache->GetResource<Texture2D>("Textures/back_beer_yellow.png");

        vector<Vec2i> positionCups;
        // faire -40: position adaptée en attendant de résoudre le porbleme de la hauteur des cups / balle
        positionCups.emplace_back(434 , 258);
        positionCups.emplace_back(491 , 258);
        positionCups.emplace_back(549 , 258);
        positionCups.emplace_back(460 , 275);
        positionCups.emplace_back(515 , 275);
        positionCups.emplace_back(491 , 294);

        for (unsigned i = 0; i < NUM_main; ++i) {

            // Create a new sprite, set it to use the texture
            SharedPtr<Sprite> sprite(new Sprite(context_));
            SharedPtr<Sprite> blueCup(new Sprite(context_));
            SharedPtr<Sprite> splash(new Sprite(context_));

            sprite->SetTexture(decalTex);
            splash->SetTexture(splashTex);
            blueCup->SetTexture(blueTex);

            // Set position of the cup
            // sprite->SetPosition(Vector2((width+i*100)/2,(height+i*100)/2));
            sprite->SetPosition(positionCups[i].x, positionCups[i].y);
            splash->SetPosition(positionCups[i].x+3, positionCups[i].y-9);
            blueCup->SetPosition(positionCups[i].x, positionCups[i].y);

            // Set sprite size
            sprite->SetSize(IntVector2(56, 60));
            splash->SetSize(IntVector2(80, 23));
            blueCup->SetSize(IntVector2(56, 60));

            // Set additive blending mode
            sprite->SetBlendMode(BLEND_ALPHA);
            splash->SetBlendMode(BLEND_ALPHA);
            blueCup->SetBlendMode(BLEND_ALPHA);

            //Set priority
            if (i < 3){
                sprite->SetPriority(210);
                splash->SetPriority(211);
                blueCup->SetPriority(210);
            }
            else if (i < 5){
                sprite->SetPriority(220);
                splash->SetPriority(221);
                blueCup->SetPriority(220);
            }
            else{
                sprite->SetPriority(230);
                splash->SetPriority(231);
                blueCup->SetPriority(230);
            }


            // Add as a child of the root UI element
            ui->GetRoot()->AddChild(sprite);
            //std::cout << "Position of cup " << i << " : " << ui->GetRoot()->FindChild(sprite) << std::endl;

            // Store sprite's velocity as a custom variable
            sprite->SetVar(VAR_VELOCITY, Vector2(Random(200.0f) - 100.0f, Random(200.0f) - 100.0f));
            splash->SetVar(VAR_VELOCITY, Vector2(Random(200.0f) - 100.0f, Random(200.0f) - 100.0f));
            blueCup->SetVar(VAR_VELOCITY, Vector2(Random(200.0f) - 100.0f, Random(200.0f) - 100.0f));

            // Store main to our own container for easy movement update iteration
            main_.Push(sprite);
            splash_.Push(splash);
            bluecups_.Push(blueCup);
/*            if (*currentPlayer_ == computer_){
                usleep(2000000);
                SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(main, HandleUpdate));
            }*/
            count=0;

        }
    }else{
        //cout << endl << "Display cups player : " << currentPlayer_->getName() << " from Handle mouse" << endl;
        if (cupScored != -1){
            ui->GetRoot()->AddChild(uielem_[11]);
        }
        for (unsigned i = 0; i < main_.Size(); ++i) {
            if(player.getName().compare("Sarah") != 0) {
                ui->GetRoot()->RemoveChild(uielem_[5]);
                ui->GetRoot()->AddChild(bg_[0]);
                if (player.getCup(i).isOnTable()) {
                    ui->GetRoot()->AddChild(bluecups_[i]);
                    ui->GetRoot()->RemoveChild(splash_[i]);
                    ui->GetRoot()->RemoveChild(main_[i]);
                } else {
                    ui->GetRoot()->RemoveChild(bluecups_[i]);
                    ui->GetRoot()->RemoveChild(splash_[i]);
                    ui->GetRoot()->RemoveChild(main_[i]);
                }
            }else{
                ui->GetRoot()->AddChild(uielem_[5]);
                ui->GetRoot()->RemoveChild(bg_[0]);
                if (player.getCup(i).isOnTable()) {
                    ui->GetRoot()->AddChild(main_[i]);
                    ui->GetRoot()->RemoveChild(splash_[i]);
                    ui->GetRoot()->RemoveChild(bluecups_[i]);
                } else {
                    ui->GetRoot()->RemoveChild(main_[i]);
                    ui->GetRoot()->RemoveChild(splash_[i]);
                    ui->GetRoot()->RemoveChild(bluecups_[i]);
                }
            }
        }
        cout << endl;
/*        if (*currentPlayer_ == computer_){
            usleep(2000000);
            SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(main, HandleUpdate));
        }*/
        count=0;
    }
    cout << "Put count to 0." << endl;
    count = 0;
}

void main::CreateReturnButton()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();

    if(uielem_.Size() < 12) {
        //Create return button
        XMLFile *style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
        // Set the loaded style as default style
        ui->GetRoot()->SetDefaultStyle(style);
        // Create a Button
        SharedPtr<Button> buttonReturn(new Button(context_));
        ui->GetRoot()->AddChild(buttonReturn);
        buttonReturn->SetName("ButtonReturn");
        buttonReturn->SetMinHeight(30);
        buttonReturn->SetMinWidth(100);
        buttonReturn->SetPosition(5,5);
        // Set the text displayed on the button
        Font *font = cache->GetResource<Font>("Fonts/Roboto-Bold.ttf");
        Text *buttonText = buttonReturn->CreateChild<Text>();
        buttonText->SetFont(font, 14);
        buttonText->SetAlignment(HA_CENTER, VA_CENTER);
        buttonText->SetText("Return");
        Color *c = new Color(0.25, 0.25, 0.25, 1.0);
        buttonText->SetColor(*c);
        // Set customised style done in xml
        buttonReturn->SetStyle("ButtonCust");
        buttonReturn->SetPriority(520);
        uielem_.Push(buttonReturn);
    }else{
        ui->GetRoot()->AddChild(uielem_[10]);
    }
    SubscribeToEvent(uielem_[10], E_RELEASED, URHO3D_HANDLER(main, HandleReturnPressed));
}

void main::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    if(k<graphicsTrajectory_.size()){
        draggedElement_->SetPosition(graphicsTrajectory_[k].getX(), graphicsTrajectory_[k].getZ());
        draggedElement_->SetSize(graphicsTrajectory_[k].getY(),graphicsTrajectory_[k].getY());
        k=k+1;
        if (k > 220 || ((ballTrajectory_.back().getY() > 220) && (graphicsTrajectory_[k].getZ() + graphicsTrajectory_[k].getY() > 294)
                        && (graphicsTrajectory_[k].getZ() > graphicsTrajectory_[k-1].getZ())))
            draggedElement_->SetPriority(205);
        else if (k > 198 || ((ballTrajectory_.back().getY() > 198) && (graphicsTrajectory_[k].getZ() + graphicsTrajectory_[k].getY() > 275)
                        && (graphicsTrajectory_[k].getZ() > graphicsTrajectory_[k-1].getZ())))
            draggedElement_->SetPriority(215);
        else if (k > 176 || ((ballTrajectory_.back().getY() > 176) && (graphicsTrajectory_[k].getZ() + graphicsTrajectory_[k].getY() > 258)
                        && (graphicsTrajectory_[k].getZ() > graphicsTrajectory_[k-1].getZ())))
            draggedElement_->SetPriority(225);
        else
            draggedElement_->SetPriority(410);

        if (ballTrajectory_[k].getZ() < 0){
            draggedElement_->SetPriority(100);
        }
    } else {
        if (ballTrajectory_[ballTrajectory_.size()-1].getZ() < 0){
            draggedElement_->SetPriority(100);
        }
        graphicsTrajectory_.clear();
        ThrowResult(cupScored);
        UnsubscribeFromEvent(E_UPDATE);
        SubscribeToEvent(E_MOUSEBUTTONDOWN, URHO3D_HANDLER(main,HandleMouse));

    }
}

bool main::AnyWinner()
{
    if (sarah_.hasWon() || lucas_.hasWon() || computer_.hasWon())
        return true;
    else
        return false;
}