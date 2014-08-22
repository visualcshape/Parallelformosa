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
    CCASSERT(temp, "temp cannot be null.");
    Size baseWindowSize = temp->getContentSize();
    Rect insetRect(8,8,baseWindowSize.width-8*2,baseWindowSize.height-8*2);
    
    _baseWindow = extension::Scale9Sprite::create("dialogueWindowBase.png", Rect(0, 0, baseWindowSize.width, baseWindowSize.height), insetRect);
    CCASSERT(_baseWindow!=nullptr, "_baseWindow cannot be null");
    _baseWindow->setAnchorPoint(Vec2(.5, .5));
    _baseWindow->setPosition(Vec2(VisibleRect::center().x,VisibleRect::center().y-20));
    
    //content first
    _contentRichText = ui::RichText::create();
    _contentRichText->ignoreContentAdaptWithSize(false);
    //Compute container size
    //ui::RichElementText* contentElement = ui::RichElementText::create(1, Color3B(255, 255, 255), 255, _content, "STHeiti Light_0",computeFontSize(6*4));
    //font problem
    ui::RichElementText* heightDefinitioner = ui::RichElementText::create(2, Color3B(255,255,255), 255, "A", "fonts/Avenir.ttf", computeFontSize(6*4));
    _contentRichText->pushBackElement(heightDefinitioner);
    _contentRichText->setPosition(Vec2(VisibleRect::getVisibleRect().size.width/2,VisibleRect::getVisibleRect().size.height/2));
    //title
    
    
    //contentBackgtound
    _contentBackground = Sprite::create();
    _contentBackground->setColor(Color3B(180,180,180));
    _contentBackground->setAnchorPoint(Vec2(.5, 1.));
    
    //default
    _baseWindow->setContentSize(baseWindowSize);
    
    //addChild...
    addChild(_baseLayer,3);
    addChild(_baseWindow,4);
    addChild(_contentRichText,5);
    
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