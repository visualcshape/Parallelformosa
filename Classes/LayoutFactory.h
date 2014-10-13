//
//  LayoutFactory.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/12.
//
//

#ifndef __Parallelformosa_Cocos2dx__LayoutFactory__
#define __Parallelformosa_Cocos2dx__LayoutFactory__

#include <cocos2d.h>
#include <CocosGUI.h>

USING_NS_CC;
USING_NS_CC::ui;

class LayoutFactory
{
public:
    static Layout* createLayout(Size size,Layout::Type type);
};

#endif /* defined(__Parallelformosa_Cocos2dx__LayoutFactory__) */
