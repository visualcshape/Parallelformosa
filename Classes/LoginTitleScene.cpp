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
#include "NoticeManager.h"
#include "LoadingScene.h"
#include "NoticeLoading.h"
#include "LoadingScene.h"
//


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
    
    //Add notice manager
    NoticeManager::getInstance()->put("UIDNotFound", "ERROR", Color3B::RED, "UID不存在，重設UID", Color3B::BLACK, nullptr);
    //
    
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
    CCLOG("Start Login!");
#if NO_CONNECTION == 0
    _doLoginWaterFall();
#else
    //Go to main scene
    auto scene = LoadingScene::createScene();
    Director::getInstance()->replaceScene(scene);
#endif
}

//Login waterfall
void MenuLayer::_doLoginWaterFall()
{
    //New connect
    _newConnectServer();
}

void MenuLayer::_newConnectServer()
{
    //test zone
    NoticeLoading* pLoading = NoticeLoading::create("Connecting...", Color3B::WHITE);
    addChild(pLoading,99,"Loading");
    //pLoading->autorelease();
    //
    
    //Timer
    schedule(schedule_selector(MenuLayer::_timedOut), 5.0f);
    //
    
    Layer* thisLayer = this;
    CCPomeloWrapper::getInstance()->connectAsnyc(GATE_HOST, GATE_PORT,[=](int err){
        thisLayer->unschedule(schedule_selector(MenuLayer::_timedOut));
        if(err!=0)
        {
            thisLayer->removeChildByName("Loading");
            pLoading->autorelease();
            DialogueWindowConfirm* pDialogue = DialogueWindowConfirm::create("Error", Color3B::RED, "Cannot connect to server , please check \nyour network.", Color3B::BLACK);
            auto cb = [=](Ref* pSender,ui::Widget::TouchEventType type)
                {
                    if(type==ui::Widget::TouchEventType::ENDED)
                    {
                        thisLayer->removeChildByTag(DIALAGOUE_TAG);
                        pDialogue->autorelease();
                    }
                };
            pDialogue->addButtonListener(cb);
            thisLayer->addChild(pDialogue, 100, DIALAGOUE_TAG);
            CCLOG("Connect Failed");
        }
        else
        {
            CCLOG("Connected!");
            _newSendRequest();
        }
    });
}

void MenuLayer::_newSendRequest()
{
    const char* route = "gate.gateHandler.authUIDAndDispatch";
    Json::Value msg;
    Json::FastWriter writer;
    msg["uid"] = LoginTitleModel::getInstance()->getUID();
    
    auto cb = std::bind(&MenuLayer::_newOnAuthUIDRequestCallback, this,std::placeholders::_1);
    CCPomeloWrapper::getInstance()->request(route, writer.write(msg), cb);
}

void MenuLayer::_newOnAuthUIDRequestCallback(const CCPomeloRequestResult& result)
{
    CCLOG("OnAuthUIDCallback...");
    Json::Value root;
    Json::Reader reader;
    if(reader.parse(result.jsonMsg, root))
    {
        CCLOG("Server response :\n %s",root.toStyledString().c_str());
        if(!root["reps"]){
            Json::Value resp = root["resp"];
            std::string type = resp["type"].asString();
            if(type=="create")
            {
                std::string uid = resp["uid"].asString();
                LoginTitleModel::getInstance()->setUID(uid);
                _startLodaing(resp);
            }
            else if(type=="isExist")
            {
                //true
                if(resp["result"].asBool())
                {
                    _startLodaing(resp);
                }
                else
                {
                    Layer* thisLayer = this;
                    DialogueWindowConfirm* pDialogue = DialogueWindowConfirm::create("Error", Color3B::RED, "UID not existed.\nReset UID.", Color3B::BLACK);
                    auto cb = [=](Ref* pSender,ui::Widget::TouchEventType type)
                    {
                        if(type==ui::Widget::TouchEventType::ENDED)
                        {
                            thisLayer->removeChildByTag(DIALAGOUE_TAG);
                            //remove loading
                            thisLayer->removeChildByName("Loading");
                            pDialogue->autorelease();
                            LoginTitleModel::getInstance()->setUID("0");
                        }
                    };
                    pDialogue->addButtonListener(cb);
                    addChild(pDialogue,100,DIALAGOUE_TAG);
                    CCPomeloWrapper::getInstance()->stop();
                }
            }
        }else if(root["code"]!=nullptr)
        {
            if(root["code"].asInt()==500)
            {
                Layer* thisLayer = this;
                DialogueWindowConfirm* pDialogue = DialogueWindowConfirm::create("Error", Color3B::RED, "Unexpected error occured.", Color3B::BLACK);
                auto cb = [=](Ref* pSender,ui::Widget::TouchEventType type)
                {
                    if(type==ui::Widget::TouchEventType::ENDED)
                    {
                        thisLayer->removeChildByTag(DIALAGOUE_TAG);
                        //remove loading
                        thisLayer->removeChildByName("Loading");
                        pDialogue->autorelease();
                    }
                };
                pDialogue->addButtonListener(cb);
                addChild(pDialogue,100,DIALAGOUE_TAG);
                CCPomeloWrapper::getInstance()->stop();
            }
        }else
        {
            Layer* thisLayer = this;
            DialogueWindowConfirm* pDialogue = DialogueWindowConfirm::create("Error", Color3B::RED, "Unexpected error occured.", Color3B::BLACK);
            auto cb = [=](Ref* pSender,ui::Widget::TouchEventType type)
            {
                if(type==ui::Widget::TouchEventType::ENDED)
                {
                    thisLayer->removeChildByTag(DIALAGOUE_TAG);
                    thisLayer->removeChildByName("Loading");
                    pDialogue->autorelease();
                }
            };
            pDialogue->addButtonListener(cb);
            addChild(pDialogue,100,DIALAGOUE_TAG);
            CCPomeloWrapper::getInstance()->stop();
        }
    }
}

void MenuLayer::_startLodaing(Json::Value resp)
{
    CONNECTOR_HOST = resp["connectorHost"].asString();
    CONNECTOR_PORT = resp["connectorPort"].asInt();
    CCPomeloWrapper::getInstance()->stop();
    Scene* pScene = LoadingScene::createScene();
    auto fadeTransition = TransitionFade::create(0.32f, pScene, Color3B(0,0,0));
    Director::getInstance()->replaceScene(fadeTransition);
}

void MenuLayer::_timedOut(float delta)
{
    unschedule(schedule_selector(MenuLayer::_timedOut));
    CCLOG("Connect timed out");
    //stop
    if(CCPomeloWrapper::getInstance()->status()==CCPomeloStatus::EPomeloConnecting)
    {
        CCPomeloWrapper::getInstance()->stop();
    }
    //
    Layer* thisLayer = this;
    removeChildByName("Loading");
    DialogueWindowConfirm* pDialogue = DialogueWindowConfirm::create("Error", Color3B::RED, "Connection timed out,configuration may goes wrong.", Color3B::BLACK);
    auto cb = [=](Ref* pSender,ui::Widget::TouchEventType type)
    {
        if(type==ui::Widget::TouchEventType::ENDED)
        {
            thisLayer->removeChildByTag(DIALAGOUE_TAG);
            pDialogue->autorelease();
        }
    };
    pDialogue->addButtonListener(cb);
    addChild(pDialogue, 100, DIALAGOUE_TAG);
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

/////////////
//InfoLayer//
/////////////

InfoLayer::InfoLayer()
{
    //Add Subject
    _subject = LoginTitleModel::getInstance();
    _subject->Attach(this);
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
