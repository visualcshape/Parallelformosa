//
//  DialogueWindowConfirm.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/8/20.
//
//

#include "DialogueWindowConfirm.h"

DialogueWindowConfirm::DialogueWindowConfirm()
{
    DialogueWindow::init();
}

DialogueWindowConfirm::~DialogueWindowConfirm()
{
    
}

DialogueWindowConfirm* DialogueWindowConfirm::create(std::string title,cocos2d::Color4B titleColor,std::string content,cocos2d::Color4B contentColor)
{
    DialogueWindowConfirm* ret = new DialogueWindowConfirm();
    
    ret->setTitleText(title);
    ret->setContentText(content);
    ret->setTitleColor(titleColor);
    ret->setContentColor(contentColor);
    
    return ret;
}

void DialogueWindowConfirm::setContentText(std::string text)
{
    _content = text;
}

void DialogueWindowConfirm::setTitleText(std::string text)
{
    _title = text;
}

void DialogueWindowConfirm::setTitleColor(cocos2d::Color4B color)
{
    _titleColor = color;
}

void DialogueWindowConfirm::setContentColor(cocos2d::Color4B color)
{
    _contentColor = color;
}