//
//  LayoutFactory.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/12.
//
//

#include "LayoutFactory.h"

Layout* LayoutFactory::createLayout(cocos2d::Size size, Layout::Type type)
{
    Layout* ret = Layout::create();
    CC_ASSERT(ret!=nullptr);
    ret->setContentSize(size);
    ret->setLayoutType(type);
    
    return ret;
}