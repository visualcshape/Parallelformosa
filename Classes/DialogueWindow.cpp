//
//  DialogueWindow.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/8/19.
//
//

#include "DialogueWindow.h"
#include "AppMacro.h"
#include "VisibleRect.h"
#include "AppMacro.h"
#include <math.h>

USING_NS_CC;

DialogueWindow::DialogueWindow()
{
    
}

DialogueWindow::~DialogueWindow()
{
    
}

bool DialogueWindow::init()
{
    bool ret = true;
    
    _baseLayer = LayerColor::create(Color4B(0, 0, 0, 255*0.75), VisibleRect::getVisibleRect().size.width,VisibleRect::VisibleRect::getVisibleRect().size.height);
    CCASSERT(_baseLayer!=nullptr, "_baseLayer cannot be null");
    _baseLayer->setAnchorPoint(Vec2::ZERO);
    _baseLayer->setPosition(Vec2(0,0));
    
    
    //base window here
    Sprite* temp = Sprite::create("dialogueWindowBase.png");
    //retain this sprite
    temp->retain();
    CCASSERT(temp, "temp cannot be null.");
    Size baseWindowSize = temp->getContentSize();
    Rect insetRect(8,8,baseWindowSize.width-8*2,baseWindowSize.height-8*2);
    //release
    temp->release();
    
    _baseWindow = extension::Scale9Sprite::create("dialogueWindowBase.png", Rect(0, 0, baseWindowSize.width, baseWindowSize.height), insetRect);
    CCASSERT(_baseWindow!=nullptr, "_baseWindow cannot be null");
    _baseWindow->setAnchorPoint(Vec2(.5, .5));
    _baseWindow->setPosition(Vec2(VisibleRect::center().x,VisibleRect::center().y-20));
    
    //default
    _baseWindow->setContentSize(Size(VisibleRect::getVisibleRect().size.width/2, VisibleRect::getVisibleRect().size.height/2+VisibleRect::getVisibleRect().size.height/5));
    Size baseWindowSizeSetted = _baseWindow->getContentSize();
    
    //Set title
    _titleText = ui::Text::create(_title, "Avenir", computeFontSize(10*4));
    CCASSERT(_titleText!=nullptr, "_titleText cannot be null.");
    _titleText->setAnchorPoint(Vec2(.5, 1.));
    _titleText->setPosition(Vec2(baseWindowSizeSetted.width/2, baseWindowSizeSetted.height-8));
    _titleText->setColor(_titleColor);
    _baseWindow->addChild(_titleText);
    
    //titleText item height
    float titleTextHeight = _titleText->getContentSize().height;
    Vec2 titlePos = _titleText->getPosition();
    
    //Content background Pos
    Vec2 contentBackgroundPos = Vec2(titlePos.x,titlePos.y-titleTextHeight-5);
    
    //content background
    _contentBackground = Sprite::create();
    CCASSERT(_contentBackground!=nullptr, "_contentBackground cannot be null.");
    _contentBackground->setTextureRect(Rect(0,0,baseWindowSizeSetted.width-10,baseWindowSizeSetted.height-titleTextHeight-8-baseWindowSizeSetted.height/4));
    _contentBackground->setColor(Color3B(150, 150, 150));
    //_contentBackground->setContentSize(Size(baseWindowSizeSetted.width-10,baseWindowSizeSetted.height-baseWindowSizeSetted.height/15));
    _contentBackground->setAnchorPoint(Vec2(.5, 1.));
    _contentBackground->setPosition(contentBackgroundPos);
    _contentBackground->setOpacity(120);
    _baseWindow->addChild(_contentBackground,0);
    
    Size contentBackgroundSize = _contentBackground->getContentSize();
    //Content
    _contentRichText = ui::RichText::create();
    _contentRichText->ignoreContentAdaptWithSize(false);
    _contentRichText->setContentSize(Size(contentBackgroundSize.width-4-2,contentBackgroundSize.height-4-2));
    ui::RichElementText* contentElement = ui::RichElementText::create(1, _contentColor, 255, _content, "STHeiti Light0", computeFontSize(6*4));
    _contentRichText->pushBackElement(contentElement);
    _contentRichText->setAnchorPoint(Vec2(.5, 1.));
    _contentRichText->setPosition(Vec2(contentBackgroundPos.x+2,contentBackgroundPos.y+10+abs((contentBackgroundSize.height-contentBackgroundPos.y))));
    _baseWindow->addChild(_contentRichText,1);
    
    //addChild...
    addChild(_baseLayer,3);
    addChild(_baseWindow,4);
    
    return ret;
}

extension::Scale9Sprite* DialogueWindow::getBaseWindow()
{
    return _baseWindow;
}

cocos2d::ui::Text* DialogueWindow::getTitleText()
{
    return _titleText;
}

cocos2d::ui::RichText* DialogueWindow::getContentRichText()
{
    return _contentRichText;
}

cocos2d::Sprite* DialogueWindow::getContentBackground()
{
    return _contentBackground;
}

void DialogueWindow::setTitleText(std::string text)
{
    _title = text;
}

void DialogueWindow::setContentText(std::string text)
{
    _content = text;
}

void DialogueWindow::setTitletextColor(cocos2d::Color3B color)
{
    _titleColor = color;
}

void DialogueWindow::setContentTextColor(cocos2d::Color3B color)
{
    _contentColor = color;
}