//
//  CustomLabelMenuItem.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/29.
//
//

#include "CustomLabelMenuItem.h"

USING_NS_CC;

const unsigned int kClickAction = 0x1;

CustomMenuItemLabel* CustomMenuItemLabel::create(cocos2d::Node *label, const cocos2d::ccMenuCallback &callback)
{
    CustomMenuItemLabel* ret = new CustomMenuItemLabel();
    ret->initWithLabel(label, callback);
    ret->autorelease();
    
    return ret;
}

void CustomMenuItemLabel::activate()
{
    if(_enabled)
    {
        Action* action = getActionByTag(kClickAction);
        if(action)
        {
            this->stopAction(action);
        }
        this->setScale(_originalScale);
        MenuItem::activate();
    }
}

void CustomMenuItemLabel::selected()
{
    if(_enabled)
    {
        MenuItem::selected();
        
        Action* action = getActionByTag(kClickAction);
        if(action)
        {
            this->stopAction(action);
        }
        else
        {
            _originalScale = this->getScale();
        }
        
        Action* clickAction = ScaleTo::create(0.05f, 0.95f);
        clickAction->setTag(kClickAction);
        this->runAction(clickAction);
    }
}


void CustomMenuItemLabel::unselected()
{
    if(_enabled)
    {
        MenuItem::unselected();
        this->stopActionByTag(kClickAction);
        Action* recoverZoom = ScaleTo::create(0.05f, _originalScale);
        recoverZoom->setTag(kClickAction);
        this->runAction(recoverZoom);
    }
}