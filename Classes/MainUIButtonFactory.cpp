//
//  MainUIButtonFactory.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/9/30.
//
//

#include "MainUIButtonFactory.h"

Button* MainUIButtonFactory::create(Vec2 position, Widget::ccWidgetTouchCallback callback,string normal,string pressed){
    Button* ret = Button::create(normal,pressed);
    LinearLayoutParameter* lp = LinearLayoutParameter::create();
    lp->setGravity(LinearLayoutParameter::LinearGravity::BOTTOM);
    lp->setMargin(Margin(0, 0, 0, 0));
    ret->setPosition(position);
    ret->addTouchEventListener(callback);
    ret->setLayoutParameter(lp);
    
    return ret;
}