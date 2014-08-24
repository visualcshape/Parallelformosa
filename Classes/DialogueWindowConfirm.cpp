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

DialogueWindowConfirm* DialogueWindowConfirm::create(std::string title,cocos2d::Color3B titleColor,std::string content,cocos2d::Color3B contentColor,std::function<void(Ref*,ui::Widget::TouchEventType)>* callback)
{
    DialogueWindowConfirm* ret = new DialogueWindowConfirm();
    
    ret->setTitleText(title);
    ret->setContentText(content);
    ret->setTitletextColor(titleColor);
    ret->setContentTextColor(contentColor);
    //init this class
    ret->setCallback(callback);
    ret->init();
    
    return ret;
}

bool DialogueWindowConfirm::init()
{
    bool ret = true;
    
    //init super class first
    DialogueWindow::init();
    //
    ui::Layout* layout = DialogueWindow::getLayout();
    Size confirmButtonSize(layout->getContentSize().width/4,layout->getContentSize().height-2);
    _confirmButton = ui::Button::create();
    _confirmButton->setTouchEnabled(true);
    _confirmButton->loadTextures("BTN.png", "BTN_P.png");
    _confirmButton->setTitleFontSize(computeFontSize(10*4));
    _confirmButton->setTitleFontName("Avenir");
    _confirmButton->setScale9Enabled(true);
    _confirmButton->setTitleText("OK");
    if(_callback!=nullptr)
        _confirmButton->addTouchEventListener(*_callback);
    _confirmButton->setContentSize(confirmButtonSize);
    ui::LinearLayoutParameter* lp = ui::LinearLayoutParameter::create();
    lp->setGravity(cocos2d::ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    lp->setMargin(cocos2d::ui::Margin(10,0,10,0));
    _confirmButton->setLayoutParameter(lp);
    layout->addChild(_confirmButton);
    
    return ret;
}

void DialogueWindowConfirm::setCallback(std::function<void (Ref *,ui::Widget::TouchEventType)> *callback)
{
    _callback = callback;
}