//
//  WindowProtocol.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/1.
//
//

#include "WindowProtocol.h"
#include "AppMacro.h"

WindowProtocol::WindowProtocol(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback){
    _titleString = titleText;
    _closeCallback = closeCallback;
}

WindowProtocol::~WindowProtocol(){
    
}

WindowProtocol* WindowProtocol::create(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback){
    WindowProtocol* ret = new WindowProtocol(titleText,closeCallback);
    if(ret&&ret->init()){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool WindowProtocol::init(){
    bool ret = false;
    
    //super init
    if(!Layer::init())
        return ret;
    
    //Put blocker
    Layer* blocker = LayerColor::create(Color4B(0, 0, 0, 255*0.5), VisibleRect::getVisibleRect().size.width, VisibleRect::getVisibleRect().size.height);
    blocker->setAnchorPoint(Vec2::ZERO);
    blocker->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x, VisibleRect::getVisibleRect().origin.y));
    
    //Base window
    _baseWindow = Scale9Sprite::createWithSpriteFrameName("WindowBase.psd",BASE_WINDOW_INSET_RECT);
    _baseWindow->setContentSize(Size(850,490-10-10));
    _baseWindow->setAnchorPoint(Vec2(0.0, 1.0));
    _baseWindow->setPosition(Vec2(55, VisibleRect::getVisibleRect().size.height-60));
    
    //Put base window
    _baseLayout = Layout::create();
    CCASSERT(_baseLayout!=nullptr, "_baseLayout cannot be null.");
    _baseLayout->setLayoutType(Layout::Type::VERTICAL);
    _baseLayout->setContentSize(_baseWindow->getCapInsets().size);
    _baseLayout->setAnchorPoint(Vec2(0.0, 1.0));
    _baseLayout->setPosition(Vec2(2,_baseWindow->getContentSize().height-2));
    //PutLayout on sprite
    _baseWindow->addChild(_baseLayout,0);
    
    //---Title---
    //Layout
    _titleLayout = Layout::create();
    CCASSERT(_titleLayout!=nullptr, "_titleLayer cannot be null.");
    //_titleLayout->setAnchorPoint(Vec2(0.0, 1.0));
    _titleLayout->setLayoutType(Layout::Type::HORIZONTAL);
    _titleLayout->setContentSize(Size(850-2*2, 55));
    _titleLayout->setPosition(Vec2(_baseLayout->getContentSize().width/2, _baseLayout->getContentSize().height/2));
    //set baseTitle as titleLayout background
    //No longer need title base image...
    _titleLayout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    _titleLayout->setBackGroundColor(Color3B(200, 200, 169));
    //Close Button
    LinearLayoutParameter* lpCloseButton = LinearLayoutParameter::create();
    _closeButton = Button::create("MainUI_Windows_Close_Normal.png","MainUI_Windows_Close_Pressed.png","",TextureResType::PLIST);
    CCASSERT(_closeButton!=nullptr, "_closeButton cannot be null.");
    _closeButton->setPosition(Vec2(_titleLayout->getContentSize().width/2, _titleLayout->getContentSize().height/2));
    lpCloseButton->setGravity(LinearLayoutParameter::LinearGravity::LEFT);
    lpCloseButton->setMargin(Margin(0, 0, 0, 0));
    _closeButton->setLayoutParameter(lpCloseButton);
    if(_closeCallback!=nullptr)
        _closeButton->addTouchEventListener(_closeCallback);
    
    //Title text
    LinearLayoutParameter* lpTitleText = LinearLayoutParameter::create();
    _titleText = Text::create(_titleString, "fonts/Apple_LiGothic_Medium.ttf", computeFontSize(42));
    CCASSERT(_titleText!=nullptr, "_titletext cannot be null.");
    _titleText->setColor(Color3B(59,134,134));
    _titleText->setPosition(Vec2(_titleLayout->getContentSize().width/2, _titleLayout->getContentSize().height/2));
    lpTitleText->setGravity(LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
    lpTitleText->setMargin(Margin(_titleLayout->getContentSize().width/2-_titleText->getContentSize().width/2-_closeButton->getContentSize().width, 0, 0, 0));
    _titleText->setLayoutParameter(lpTitleText);
    
    
    //Add component to title layout
    _titleLayout->addChild(_closeButton,1);
    _titleLayout->addChild(_titleText,2);
    
    //---add to base layout---
    _baseLayout->addChild(_titleLayout);
    
    //setBlockerTouchDispatcher
    auto touchBlocker = EventListenerTouchOneByOne::create();
    touchBlocker->setSwallowTouches(true);
    //lambda decide later
    touchBlocker->onTouchBegan= [](Touch* touch,Event* e){
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchBlocker, blocker);
    
    //addChild
    addChild(blocker,0);
    addChild(_baseWindow,1);
    
    ret = true;
    return ret;
}