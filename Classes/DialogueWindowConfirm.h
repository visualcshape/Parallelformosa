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
protected:
    DialogueWindowConfirm();
public:
    ~DialogueWindowConfirm();
    CREATE_FUNC(DialogueWindowConfirm);
    //Callback will be implemented later
    static DialogueWindowConfirm* create(std::string title,cocos2d::Color3B titleColor,std::string content,cocos2d::Color3B contetntColor);
    
    virtual bool init();
};


#endif
