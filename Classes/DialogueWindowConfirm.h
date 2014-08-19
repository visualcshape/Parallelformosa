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
#include "DialogueWindow.h"

class DialogueWindowConfirm:public DialogueWindow
{
private:
    std::string _title;
    std::string _content;
    cocos2d::Color4B _titleColor;
    cocos2d::Color4B _contentColor;
protected:
    DialogueWindowConfirm();
public:
    ~DialogueWindowConfirm();
    CREATE_FUNC(DialogueWindowConfirm);
    //Callback will be implemented later
    static DialogueWindowConfirm* create(std::string title,cocos2d::Color4B titleColor,std::string content,cocos2d::Color4B contetntColor);
    
    void setTitleText(std::string text);
    
    void setContentText(std::string text);
    
    void setTitleColor(cocos2d::Color4B color);
    
    void setContentColor(cocos2d::Color4B color);
};


#endif
