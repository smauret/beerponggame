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
    if(uielem_.Size() < 10) {
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
        ui->GetRoot()->AddChild(uielem_[9]);
    }
    SubscribeToEvent(E_MOUSEBUTTONDOWN, URHO3D_HANDLER(main,HandleMouse));
    //cout << "Create draggable ball" << "  uielem_ size : " << uielem_.Size() << endl;
}
void main::HandleMouse(StringHash eventType, VariantMap& eventData)
{
    int x = GetSubsystem<Input>()->GetMousePosition().x_;
    int y = GetSubsystem<Input>()->GetMousePosition().y_;
    draggedElement_->SetSize(68,68);
    draggedElement_->SetPosition(x - (draggedElement_->GetSize().x_)/2,y - (draggedElement_->GetSize().y_)/2);
    draggedElement_->SetPriority(410);
    cout << "Displaying cups of " << currentPlayer_->getName() << endl;
    DisplayCups(*currentPlayer_);
    // Subscribe draggableBall to Drag Events (in order to make it draggable)
    // See "Event list" in documentation's Main Page for reference on available Events and their eventData
    SubscribeToEvent(draggedElement_, E_DRAGBEGIN, URHO3D_HANDLER(main, HandleDragBegin));
    SubscribeToEvent(draggedElement_, E_DRAGMOVE, URHO3D_HANDLER(main, HandleDragMove));
    SubscribeToEvent(draggedElement_, E_DRAGEND, URHO3D_HANDLER(main, HandleDragEnd));
    cout << "Current player : " << currentPlayer_->getName() << endl;
    cout << "Sarah cup on table : " << sarah_.getCup(1).isOnTable() << endl;
    cout << "Lucas cup on table : " << lucas_.getCup(1).isOnTable() << endl << endl;
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
    cout << "Handle drag end" << endl;
    IntVector2 dragCurrentPosition = IntVector2(eventData["X"].GetInt(), eventData["Y"].GetInt());
    // Calculate the power based on the time and the distance
    double speed = GetSpeed(BeginPosition_,dragCurrentPosition);
    if(speed > 100 && speed < 1500){
        double restSpeed = speed /10;
        speed = 350+restSpeed;
    }

    // Calculate the rotation angle
    double rotation_angle = GetRotation(BeginPosition_,dragCurrentPosition);

    rotation_angle = M_PI/2 + (rotation_angle - M_PI/2) / 12;

    // Calculate trajectory
    IntVector3 finalPositionCm = GetInitPosCm(dragCurrentPosition);
    cupScored = -1;
    //ballTrajectory_ = currentPlayer_.throwBall(M_PI/4, rotation_angle, (double)(finalPositionCm.z_), speed*100, (double)(finalPositionCm.x_), 0, cupScored);
    ballTrajectory_ = currentPlayer_->throwBall(M_PI/4, rotation_angle, 50, speed, (double)(finalPositionCm.x_), 0, cupScored);
    //ballTrajectory_ = currentPlayer_->throwBall (static_cast<double>(M_PI / 4), static_cast<double>(M_PI / 2), 50, 420, 30, 0, cupScored);

    std::cout << "Cup scored " << cupScored << std::endl << std::endl;
    for (int i=0; i<ballTrajectory_.size(); i++) {
        graphicsTrajectory_.emplace_back(1024/2, 768/2,0);
    }
    currentPlayer_->get_xzSize_graphics(ballTrajectory_, graphicsTrajectory_);
    k=0;
    UnsubscribeFromEvent(E_DRAGBEGIN);
    UnsubscribeFromEvent(E_DRAGMOVE);
    UnsubscribeFromEvent(E_DRAGEND);
    UnsubscribeFromEvent(E_MOUSEBUTTONDOWN);

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(main, HandleUpdate));

}

void main::ThrowResult(int cupScored){
    cout << "Throw result " << "  size uielem_ : " << uielem_.Size()<< endl;
    std::cout << "Cup scored " << cupScored << std::endl << std::endl;
    UI* ui = GetSubsystem<UI>();
    if(cupScored == -1){
        cout << "Remove text box success -1" << endl;
        ui->GetRoot()->RemoveChild(uielem_[5]);
        SharedPtr<Text> textUpdate(new Text(context_));
        string welcome = "Welcome to the beer pong game "+currentPlayer_->getName()+" \nFailed = +0 Point";
        textUpdate->SetText(welcome.c_str());
        textUpdate->SetStyleAuto();
        textUpdate->SetOpacity(1.0);
        textUpdate->SetFont("Fonts/Anonymous Pro.ttf",30);
        textUpdate->SetPosition(0,300);
        textUpdate->SetBringToBack(true);
        uielem_[5]->RemoveAllChildren();
        uielem_[5]->AddChild(textUpdate);
        ui->GetRoot()->AddChild(uielem_[5]);

    }else{
        cout << "Remove text box success" << endl;
        ui->GetRoot()->RemoveChild(uielem_[5]);

        SharedPtr<Text> textUpdate(new Text(context_));
        string welcome = "Welcome to the beer pong game "+ currentPlayer_->getName() + " \nSuccess = +1 Point";
        textUpdate->SetText(welcome.c_str());

        textUpdate->SetStyleAuto();
        textUpdate->SetOpacity(1.0);
        textUpdate->SetFont("Fonts/Anonymous Pro.ttf",30);
        textUpdate->SetPosition(0,300);
        textUpdate->SetBringToBack(true);
        uielem_[5]->RemoveAllChildren();
        uielem_[5]->AddChild(textUpdate);
        ui->GetRoot()->AddChild(uielem_[5]);
        //ui->GetRoot()->RemoveChild(main_[cupScored]);
        ui->GetRoot()->AddChild(splash_[cupScored]);
        currentPlayer_->getCup(cupScored).setOnTable(false);
        cout << "Sarah " << sarah_.getCup(cupScored).isOnTable() << endl;
        cout << "Lucas " << lucas_.getCup(cupScored).isOnTable() << endl;
        currentPlayer_->setCupsLeft(currentPlayer_->getCupsLeft()-1);
    }
    cout << currentPlayer_->getName() << "  cups left " << currentPlayer_->getCupsLeft() << " Sarah : " << sarah_.getCupsLeft() << "  Lucas : " << lucas_.getCupsLeft() << endl;

    if (currentPlayer_->getCupsLeft() == 0)
    {
        Graphics* graphics = GetSubsystem<Graphics>();
        float width = (float)graphics->GetWidth();
        float height = (float)graphics->GetHeight();
        SharedPtr<Text> title(new Text(context_));
        string win = "YES, " + currentPlayer_->getName() + " you have won the game !";
        title->SetText(win.c_str());
        ui->GetRoot()->AddChild(title);
        title->SetStyleAuto();
        title->SetOpacity(1.0);
        title->SetFont("Fonts/Roboto-Bold.ttf",30);
        title->SetPosition(((int)width-title->GetSize().x_)/2,1*(int)height/5);
        title->SetPriority(1000);
    }

    if(currentPlayer_ == &sarah_)
        currentPlayer_ = &lucas_;
    else
        currentPlayer_ = &sarah_;
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
    //cout << "lancé: x : " << initPosCm.x_ << " | y : " << initPosCm.y_ << " | z : " << initPosCm.z_ << endl;
    return initPosCm;
}

void main::InitWelcomePage() {

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

    // Create a Button
    SharedPtr<Button> button(new Button(context_));
    ui->GetRoot()->AddChild(button);
    button->SetName("Button");
    button->SetMinHeight(53);
    button->SetMinWidth(234);
    button->SetPosition(399,563);
    // Set the text displayed on the button
    Font* font = cache->GetResource<Font>("Fonts/Roboto-Bold.ttf");
    Text* buttonText = button->CreateChild<Text>();
    buttonText->SetFont(font, 24);
    buttonText->SetAlignment(HA_CENTER, VA_CENTER);
    buttonText->SetText("Play");
    Color *c = new Color(0.25, 0.25, 0.25, 1.0);
    buttonText->SetColor(*c);
    // Apply custom style we created in xml
    button->SetStyle("ButtonCust");
    button->SetPriority(100);
    uielem_.Push(button);

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
}

void main::HandleReturnPressed(StringHash eventType, VariantMap& eventData)
{
    cout << "Handle press return" << "   uielem_ size : " << uielem_.Size() << endl;
    UI* ui = GetSubsystem<UI>();
    // Remove play screen
    ui->GetRoot()->RemoveAllChildren();
    //Add Background image
    ui->GetRoot()->AddChild(uielem_[0]);
    // Add Button
    ui->GetRoot()->AddChild(uielem_[1]);
    // Add Title
    ui->GetRoot()->AddChild(uielem_[2]);
}

void main::HandlePlayPressed(StringHash eventType, VariantMap& eventData)
{
    //cout << "event press play" << "   uielem_ size : " << uielem_.Size() << endl;
    //Player[2] players;
    // Create player
    lucas_ = Player("Lucas",6);
    sarah_ = Player("Sarah",6);
    // Graphics
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
    if(uielem_.Size() < 10){
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
        ui->GetRoot()->AddChild(uielem_[4]);
    }

    currentPlayer_ = &lucas_;
    DisplayCups(*currentPlayer_);


    //Window score in boardgame
    if(uielem_.Size() < 10) {
        SharedPtr<Window> textWindow(new Window(context_));
        // Set Window size and layout settings
        // TODO : Adapter la taille de la window au contenu ?

        textWindow->SetMaxWidth(300);
        textWindow->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
        textWindow->SetPosition(0, 200);
        textWindow->SetName("textWindow");
        textWindow->SetStyleAuto();

        SharedPtr<Text> title1(new Text(context_));
        string welcome = "Welcome to the BeerPong Game " + currentPlayer_->getName() + " !";
        title1->SetText(welcome.c_str());
        title1->SetStyleAuto();
        title1->SetOpacity(1.0);
        title1->SetFont("Fonts/Anonymous Pro.ttf", 30);
        title1->SetPosition(0, 100);
        title1->SetBringToBack(true);
        uielem_.Push(textWindow);
        textWindow->AddChild(title1);
        uielem_.Push(title1);
        textWindow->SetPriority(310);
        ui->GetRoot()->AddChild(textWindow);
    }else{
        uielem_[5]->RemoveAllChildren();
        uielem_[5]->AddChild(uielem_[6]);
        ui->GetRoot()->AddChild(uielem_[5]);
    }
    //cout << endl << "Init board game" << "   size uielem_ : " << uielem_.Size() << endl;

    CreateReturnButton();
}

void main::DisplayCups(Player player) {
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();

    if(main_.Size() == 0) {
        Texture2D *decalTex = cache->GetResource<Texture2D>("Textures/back_beer.png");
        Texture2D *splashTex = cache->GetResource<Texture2D>("Textures/beer_splash.png");
        Texture2D *blueTex = cache->GetResource<Texture2D>("Textures/back_beer copie.png");

        vector<Vec2i> positionCups;
        positionCups.emplace_back(434 , 238);
        positionCups.emplace_back(491 , 238);
        positionCups.emplace_back(549 , 238);
        positionCups.emplace_back(460 , 255);
        positionCups.emplace_back(515 , 255);
        positionCups.emplace_back(491 , 274);

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
            sprite->SetSize(IntVector2(56, 84));
            splash->SetSize(IntVector2(80, 23));
            blueCup->SetSize(IntVector2(56, 84));

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
        }
    }else{
        cout << endl << "Display cups player : " << currentPlayer_->getName() << " from Handle mouse" << endl;
        for (unsigned i = 0; i < main_.Size(); ++i) {
            if(player.getName().compare("Sarah") != 0) {
                ui->GetRoot()->AddChild(uielem_[3]);
                ui->GetRoot()->RemoveChild(bg_[0]);
                if (player.getCup(i).isOnTable()) {
                    ui->GetRoot()->AddChild(main_[i]);
                    ui->GetRoot()->RemoveChild(splash_[i]);
                    ui->GetRoot()->RemoveChild(splash_[i]);
                    ui->GetRoot()->RemoveChild(bluecups_[i]);
                } else {
                    ui->GetRoot()->RemoveChild(main_[i]);
                    ui->GetRoot()->RemoveChild(splash_[i]);
                    ui->GetRoot()->RemoveChild(bluecups_[i]);
                }
            }else{
                ui->GetRoot()->RemoveChild(uielem_[3]);
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
            }
        }
        cout << endl;
    }
}


void main::CreateReturnButton(){

    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();

    if(uielem_.Size() < 10) {
        //Create return button
        XMLFile *style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
        // Create the Window and add it to the UI's root node
        SharedPtr<Window> windowReturn_(new Window(context_));


        // Set the loaded style as default style
        ui->GetRoot()->SetDefaultStyle(style);

        // Set Window size and layout settings
        windowReturn_->SetMinWidth(100);
        windowReturn_->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
        windowReturn_->SetAlignment(HA_LEFT, VA_TOP);
        windowReturn_->SetName("Window");
        windowReturn_->SetStyleAuto();
        uielem_.Push(windowReturn_);

        // Create a Button
        SharedPtr<Button> buttonReturn(new Button(context_));
        // Add controls to Window
        windowReturn_->AddChild(buttonReturn);
        buttonReturn->SetName("ButtonReturn");
        buttonReturn->SetMinHeight(24);
        // Set the text displayed on the button
        Font *font = cache->GetResource<Font>("Fonts/Anonymous Pro.ttf");
        Text *buttonText = buttonReturn->CreateChild<Text>();
        buttonText->SetFont(font, 12);
        buttonText->SetAlignment(HA_CENTER, VA_CENTER);
        buttonText->SetText("RETURN");
        // Apply previously set default style
        buttonReturn->SetStyleAuto();
        Color *c = new Color(1.0, 0.0, 0.0, 1.0);
        buttonReturn->SetColor(*c);
        windowReturn_->SetPriority(520);
        ui->GetRoot()->AddChild(windowReturn_);
        uielem_.Push(buttonReturn);
    }else{
        uielem_[7]->RemoveAllChildren();
        uielem_[7]->AddChild(uielem_[8]);
        ui->GetRoot()->AddChild(uielem_[7]);
    }
    SubscribeToEvent(uielem_[8], E_RELEASED, URHO3D_HANDLER(main, HandleReturnPressed));
    //cout << "Create return button" << "   size uielem_ : " << uielem_.Size() << endl;
    //ui->GetRoot()->RemoveChild(uielem_[6]);
}

void main::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    if(k<graphicsTrajectory_.size()){
        draggedElement_->SetPosition(graphicsTrajectory_[k].getX(), graphicsTrajectory_[k].getZ());
        draggedElement_->SetSize(graphicsTrajectory_[k].getY(),graphicsTrajectory_[k].getY());
        k=k+1;
        if (k > 225)
            draggedElement_->SetPriority(205);
        else if (k > 216)
            draggedElement_->SetPriority(215);
        else if (k > 208)
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
