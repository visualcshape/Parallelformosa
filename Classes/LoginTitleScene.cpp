//
//  LoginTitleScene.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/28.
//
//

#include "LoginTitleScene.h"
#include "VisibleRect.h"
#include "AppMacro.h"
#include <pomelo.h>
#include "DialogueWindowConfirm.h"

using namespace cocos2d;

//////////////////////////
//class LoginTitleScene //
//////////////////////////

Scene* LoginTitleScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer =LoginTitleScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool LoginTitleScene::init()
{
    //super init first
    if(!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    //Put item
    auto backgroundLayer = new BackgroundLayer();
    auto menuLayer = new MenuLayer();
    auto infoLayer = new InfoLayer();
    
    backgroundLayer->setPosition(Vec2(visibleOrigin.x, visibleOrigin.y));
    
    menuLayer->setPosition(Vec2(visibleOrigin.x, visibleOrigin.y));
    
    infoLayer->setPosition(Vec2(visibleOrigin.x, visibleOrigin.y));
    
    addChild(backgroundLayer, 0);
    addChild(menuLayer,1);
    addChild(infoLayer,1);
    
    //Layer is an autorelease object
    backgroundLayer->release();
    menuLayer->release();
    infoLayer->release();
    
    return true;
}

///////////////////
//BackgroundLayer//
///////////////////

BackgroundLayer::BackgroundLayer()
{
    
}

/////////////
//MenuLayer//
/////////////

MenuLayer::MenuLayer()
{
    m_aboutMenuItemImage = MenuItemImage::create("LoginTitle/about_normal.png", "LoginTitle/about_selected.png", "LoginTitle/about_disabled.png", CC_CALLBACK_1(MenuLayer::aboutClickCallback, this));
    CC_ASSERT(m_aboutMenuItemImage!=NULL);
    m_aboutMenuItemImage->setAnchorPoint(Vec2(0.0, 1.0));
    m_aboutMenuItemImage->setPosition(Vec2(750,VisibleRect::getVisibleRect().origin.y+100));
    
    m_settingMenuItemImage = MenuItemImage::create("LoginTitle/setting_normal.png", "LoginTitle/setting_selected.png", "LoginTitle/setting_disabled.png", CC_CALLBACK_1(MenuLayer::settingClickCallback, this));
    CC_ASSERT(m_settingMenuItemImage!=NULL);
    m_settingMenuItemImage->setAnchorPoint(Vec2(0.0, 1.0));
    m_settingMenuItemImage->setPosition(Vec2(855, VisibleRect::getVisibleRect().origin.y+100));
    
    TTFConfig ttfConfig("fonts/Silom.ttf",computeFontSize(18*4));
    m_touchToStartLabel = Label::createWithTTF(ttfConfig, "Touch to Start", TextHAlignment::CENTER);
    CC_ASSERT(m_touchToStartLabel!=nullptr);

    m_touchToStartMenuItemLabel = CustomMenuItemLabel::create(m_touchToStartLabel, CC_CALLBACK_1(MenuLayer::startClickCallback, this));
    CC_ASSERT(m_touchToStartMenuItemLabel!=NULL);
    m_touchToStartMenuItemLabel->setAnchorPoint(Vec2(0.5, 0.5));
    m_touchToStartMenuItemLabel->setPosition(Vec2(VisibleRect::getVisibleRect().size.width/2, VisibleRect::getVisibleRect().origin.y+180));
    //run action
    loopTouchToStartMenuItemLabel();
    
    //A transparent sprite..
    auto transparentSprite = Sprite::create();
    CC_ASSERT(transparentSprite!=nullptr);
    transparentSprite->setTextureRect(Rect(VisibleRect::getVisibleRect().origin.x,VisibleRect::getVisibleRect().origin.y,VisibleRect::getVisibleRect().size.width,VisibleRect::getVisibleRect().size.height));
    
    //Add transparent sprite as menu item
    m_transparentStartSprite = MenuItemSprite::create(transparentSprite, NULL);
    CC_ASSERT(m_transparentStartSprite!=NULL);
    m_transparentStartSprite->setCallback(CC_CALLBACK_1(MenuLayer::startClickCallback, this));
    m_transparentStartSprite->setOpacity(0);
    
    //menu
    m_menu = Menu::create(m_aboutMenuItemImage, m_settingMenuItemImage,m_touchToStartMenuItemLabel,nullptr);
    CC_ASSERT(m_menu!=nullptr);
    m_menu->setAnchorPoint(Vec2::ZERO);
    m_menu->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x,VisibleRect::getVisibleRect().origin.y));
    addChild(m_menu,1);
    
    //Menu for transparents sprite
    m_menuForTransparentSpriteMenuItem = Menu::create(m_transparentStartSprite, NULL);
    CC_ASSERT(m_menuForTransparentSpriteMenuItem!=nullptr);
    addChild(m_menuForTransparentSpriteMenuItem,0);
    
}

void MenuLayer::aboutClickCallback(cocos2d::Ref *pSender)
{
    
}

void MenuLayer::settingClickCallback(cocos2d::Ref *pSender)
{
    
}

void MenuLayer::startClickCallback(cocos2d::Ref *pSender)
{
    CCLOG("Start Touched!");
    //bind
    std::function<void(Ref*,cocos2d::ui::Widget::TouchEventType)> bind(std::bind(&MenuLayer::dialogueCallback, this,std::placeholders::_2));
    DialogueWindowConfirm* dialogue = DialogueWindowConfirm::create("Test", Color3B(0, 0, 0), "Hello World", Color3B(0,0,0),&bind);
    addChild(dialogue,100,"Dialogue");
    //
    //_doLoginWaterFall();
}

//Login waterfall
void MenuLayer::_doLoginWaterFall()
{
    //new a client
    pc_client_t* client = pc_client_new();
    _connectServer(client);
}

void MenuLayer::_connectServer(pc_client_t* client)
{
    const char* ip = GATE_HOST;
    const int port = GATE_PORT;
    struct sockaddr_in address;
    
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);
    
    if(pc_client_connect(client, &address))
    {
        //Future : Print on the UI
        //Now : log at Console
        CCLOGERROR("Fail to connect to server.");
        pc_client_destroy(client);
        return;
    }
    
    //Print on Console
    CCLOG("Connect to server.");
    //pass to next step
    _sendRequest(client);
}

void MenuLayer::_sendRequest(pc_client_t* client)
{
    const char* route = "gate.gateHandler.authUID";
    json_t* msg = json_object();
    json_t* uid = json_string("0");
    json_object_set(msg, "uid", uid);
    //decref
    json_decref(uid);
    
    pc_request_t* req = pc_request_new();
    pc_request(client, req, route, msg, _onAuthUIDRequestCallback);
}

void MenuLayer::_onAuthUIDRequestCallback(pc_request_t* req,int status,json_t* resp)
{
    if(status==-1)
    {
        CCLOG("Fail to send request to server");
    }
    else if(status==0)
    {
        char* json_str = json_dumps(resp, 0);
        if(json_str!=nullptr)
        {
            CCLOG("server response:%s",json_str);
            free(json_str);
        }
    }
    
    //release
    json_t* msg = req->msg;
    pc_client_t* client = req->client;
    json_decref(msg);
    pc_request_destroy(req);
    CCLOG("Released.");
    
    //stop
    pc_client_stop(client);
}
//

void MenuLayer::loopTouchToStartMenuItemLabel()
{
    Color3B originalColor = m_touchToStartMenuItemLabel->getColor();
    auto toAction = TintTo::create(.3f, 69, 174, 255);
    auto recoverAction = TintTo::create(.3f, originalColor.r, originalColor.g, originalColor.b);
    CallFunc* repeat = CallFunc::create(CC_CALLBACK_0(MenuLayer::loopTouchToStartMenuItemLabel, this));
    Sequence* sequence = Sequence::create(toAction,DelayTime::create(.3f),recoverAction,repeat, NULL);
    
    m_touchToStartMenuItemLabel->runAction(sequence);
}

void MenuLayer::dialogueCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    ((Layer*)pSender)->removeChildByName("Dialogue");
    return;
}

/////////////
//InfoLayer//
/////////////

InfoLayer::InfoLayer()
{
    //Add Subject
    _subject = LoginTitleModel::getInstance();
    _subject->Attach(this);
    //TODO : Set Position
    //TODO : Add child
    TTFConfig config("fonts/Avenir.ttf",computeFontSize(8*4));
    m_UIDLabel = Label::createWithTTF(config, "UID:Undefined", TextHAlignment::LEFT);
    CC_ASSERT(m_UIDLabel!=NULL);
    m_UIDLabel->setAnchorPoint(Vec2(0.0, 1.0));
    m_UIDLabel->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x+20,VisibleRect::getVisibleRect().size.height-3));
    addChild(m_UIDLabel,2);
    //Notify...
    _subject->Notify();
}

InfoLayer::~InfoLayer()
{
    _subject->Detach(this);
    _subject = nullptr;
}

void InfoLayer::Update(Subject* changedSubject)
{
    //Update uid
    if(changedSubject->getTypeName()==LoginTitleModel::getInstance()->getTypeName()){
        std::string uid = _subject->getUID();
        m_UIDLabel->setString("UID:"+uid);
    }
    return;
}