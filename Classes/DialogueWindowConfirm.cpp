//
//  DialogueWindowConfirm.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/8/20.
//
//

#include "DialogueWindowConfirm.h"
#include "AppMacro.h"

using namespace cocos2d;

DialogueWindowConfirm::DialogueWindowConfirm()
{
    
}

DialogueWindowConfirm::~DialogueWindowConfirm()
{
    
}

DialogueWindowConfirm* DialogueWindowConfirm::create(std::string title,cocos2d::Color3B titleColor,std::string content,cocos2d::Color3B contentColor)
{
    DialogueWindowConfirm* ret = new DialogueWindowConfirm();
    
    ret->setTitleText(title);
    ret->setContentText(content);
    ret->setTitletextColor(titleColor);
    ret->setContentTextColor(contentColor);
    //init this class
    ret->init();
    
    return ret;
}

bool DialogueWindowConfirm::init()
{
    bool ret = true;
    
    //init super class first
    DialogueWindow::init();
    //
    
    /*
    cocos2d::extension::Scale9Sprite* baseWindow = DialogueWindow::getBaseWindow();
    CCASSERT(baseWindow!=nullptr, "baseWindow cannot be null.");
    cocos2d::Size baseWindowSize = baseWindow->getContentSize();
    
    _titleText = cocos2d::ui::Text::create(_title, "Avenir", computeFontSize(10*4));
    CCASSERT(_titleText!=nullptr, "_titleText cannot be null.");
    _titleText->setColor(Color3B(123,157,134));
    Vec2 titlePosition(baseWindowSize.width/2,baseWindowSize.height-8);
    _titleText->setAnchorPoint(Vec2(.5,1.));
    _titleText->setPosition(titlePosition);
    baseWindow->addChild(_titleText);
    */
    
    return ret;
}