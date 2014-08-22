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
    
    
    return ret;
}