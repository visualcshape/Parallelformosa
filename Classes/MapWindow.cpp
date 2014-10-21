//
//  MapWindow.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/20.
//
//

#include "MapWindow.h"

MapWindow::MapWindow(string title,function<void(Ref*,Widget::TouchEventType)> closeCallback,function<void(Ref*,Widget::TouchEventType)> goMapButtonPressedCallback):WindowProtocol(title,closeCallback)
{
    _goMapButtonPressedCallback = goMapButtonPressedCallback;
}

MapWindow::~MapWindow()
{
    
}

MapWindow* MapWindow::create(string title, function<void (cocos2d::Ref *, Widget::TouchEventType)> closeCallback, function<void (cocos2d::Ref *, Widget::TouchEventType)> goMapCallback)
{
    MapWindow* ret = new MapWindow(title,closeCallback,goMapCallback);
    if(ret&&ret->init())
    {
        ret->autorelease();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        ret->retain();
#endif
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MapWindow::init()
{
    if(!WindowProtocol::init())
    {
        return false;
    }
    
    string fontName = "fonts/Silom.ttf";
    float fontSize = computeFontSize(24);
    
    LinearLayoutParameter* compLLP = LinearLayoutParameter::create();
    compLLP->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    compLLP->setMargin(Margin(0, 5, 0, 10));
    
    Text* subTitle = Text::create("Please Enter a Coordinate", fontName, computeFontSize(30));
    CC_ASSERT(subTitle);
    subTitle->setLayoutParameter(compLLP);
    
    
    _mapTextField = TextField::create("x.x.xx-xx", fontName, fontSize);
    CC_ASSERT(_mapTextField!=nullptr);
    _mapTextField->setMaxLengthEnabled(true);
    _mapTextField->setMaxLength(9);
    _mapTextField->setLayoutParameter(compLLP);
    //_mapTextField->setTextVerticalAlignment(TextVAlignment::CENTER);
    _mapTextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
    /*
    Layer* tfBK = LayerColor::create(Color4B(220,235,240,255), _mapTextField->getContentSize().width+50, _mapTextField->getContentSize().height);
    CC_ASSERT(tfBK!=nullptr);
    tfBK->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    tfBK->setPosition(Vec2::ZERO);
    _mapTextField->addChild(tfBK,-1);*/
    
    _buttonGoMap = Button::create("BlueButton.png","BlueButtonPressed.png","ButtonDisabled.png",Widget::TextureResType::PLIST);
    CC_ASSERT(_buttonGoMap!=nullptr);
    _buttonGoMap->setScale9Enabled(true);
    _buttonGoMap->setCapInsets(Rect(2, 2, _buttonGoMap->getContentSize().width-4, _buttonGoMap->getContentSize().height-4));
    _buttonGoMap->setContentSize(Size(200,50));
    _buttonGoMap->setTitleFontName(fontName);
    _buttonGoMap->setTitleFontSize(fontSize);
    _buttonGoMap->setTitleText("Go Map");
    _buttonGoMap->addTouchEventListener(_goMapButtonPressedCallback);
    _buttonGoMap->setLayoutParameter(compLLP);
    _buttonGoMap->setBindedObject(_mapTextField);
    
    _compLayout = Layout::create();
    _compLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _compLayout->setPosition(Vec2(_baseWindow->getContentSize().width/2,_baseWindow->getContentSize().height-100));
    _compLayout->setLayoutType(Layout::Type::VERTICAL);
    
    _compLayout->addChild(subTitle);
    _compLayout->addChild(_mapTextField);
    _compLayout->addChild(_buttonGoMap);
    
    _baseWindow->addChild(_compLayout);
    
    return true;
}

string MapWindow::getTextFieldString()
{
    return _mapTextField->getStringValue();
}