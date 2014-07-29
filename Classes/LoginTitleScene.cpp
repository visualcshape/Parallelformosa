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
    
    //When init finish , do login
    doLogin();
    
    return true;
}

void LoginTitleScene::doLogin()
{
    //SQLLite first
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
    
    //menu
    m_menu = Menu::create(m_aboutMenuItemImage, m_settingMenuItemImage,m_touchToStartMenuItemLabel,NULL);
    CC_ASSERT(m_menu!=nullptr);
    m_menu->setAnchorPoint(Vec2::ZERO);
    m_menu->setPosition(Vec2::ZERO);
    addChild(m_menu);
    
}

void MenuLayer::aboutClickCallback(cocos2d::Ref *pSender)
{
    
}

void MenuLayer::settingClickCallback(cocos2d::Ref *pSender)
{
    
}


void MenuLayer::startClickCallback(cocos2d::Ref *pSender)
{
    
}

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
    //TODO : Set Position
    //TODO : Add child
    TTFConfig config("fonts/Avenir.ttf",computeFontSize(8*4));
    m_UIDLabel = Label::createWithTTF(config, "UID:Undefined", TextHAlignment::LEFT);
    CC_ASSERT(m_UIDLabel!=NULL);
    m_UIDLabel->setAnchorPoint(Vec2(0.0, 1.0));
    m_UIDLabel->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x+20,VisibleRect::getVisibleRect().size.height-3));
    addChild(m_UIDLabel);
}
