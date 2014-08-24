//
//  DialogueWindowConfirm.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/8/20.
//
//

#ifndef Parallelformosa_Cocos2dx_DialogueWindowConfirm_h
#define Parallelformosa_Cocos2dx_DialogueWindowConfirm_h

#include <cocos2d.h>
#include <string>
#include <CocosGUI.h>
#include "DialogueWindow.h"

class DialogueWindowConfirm:public DialogueWindow
{
private:
    cocos2d::ui::Button* _confirmButton;
    std::function<void(cocos2d::Ref*,cocos2d::ui::Widget::TouchEventType)>* _callback;
protected:
    DialogueWindowConfirm();
public:
    ~DialogueWindowConfirm();
    CREATE_FUNC(DialogueWindowConfirm);
    //Callback will be implemented later
    static DialogueWindowConfirm* create(std::string title,cocos2d::Color3B titleColor,std::string content,cocos2d::Color3B contetntColor,std::function<void(cocos2d::Ref*,cocos2d::ui::Widget::TouchEventType)>* callback);
    
    virtual bool init();
    virtual void setCallback(std::function<void(Ref*,cocos2d::ui::Widget::TouchEventType)>* callback);
};


#endif
