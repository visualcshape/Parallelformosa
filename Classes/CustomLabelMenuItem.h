//
//  CustomLabelMenuItem.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/29.
//
//

#ifndef Parallelformosa_Cocos2dx_CustomLabelMenuItem_h
#define Parallelformosa_Cocos2dx_CustomLabelMenuItem_h
#include <cocos2d.h>

class CustomMenuItemLabel:public cocos2d::MenuItemLabel
{
public:
    static CustomMenuItemLabel* create(cocos2d::Node* label,const cocos2d::ccMenuCallback& callback);
    
    virtual void activate();
    virtual void selected();
    virtual void unselected();
};

#endif
