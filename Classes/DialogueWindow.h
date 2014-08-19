//
//  DialogueWindow.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/8/19.
//
//

#ifndef Parallelformosa_Cocos2dx_DialogueWindow_h
#define Parallelformosa_Cocos2dx_DialogueWindow_h

#include <cocos2d.h>
#include <string>
#include <cocos-ext.h>

class DialogueWindow:public cocos2d::Layer
{
private:
    cocos2d::extension::Scale9Sprite* _baseWindow;
    cocos2d::LayerColor* _baseLayer;
protected:
    DialogueWindow();
public:
    CREATE_FUNC(DialogueWindow);
    ~DialogueWindow();
    //virtual DialogueWindow* create() = 0;
    virtual bool init();
};

#endif
