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
    
    _baseLayer = LayerColor::create(Color4B(0, 0, 0, 255*0.8), VisibleRect::getVisibleRect().size.width,VisibleRect::VisibleRect::getVisibleRect().size.height);
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
    
    //default
    _baseWindow->setContentSize(baseWindowSize);
    //Delay to child class to add
    addChild(_baseLayer,3);
    addChild(_baseWindow,4);
    
    return ret;
}