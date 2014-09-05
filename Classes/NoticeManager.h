//
//  NoticeManager.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/9/4.
//
//

#ifndef Parallelformosa_Cocos2dx_NoticeManager_h
#define Parallelformosa_Cocos2dx_NoticeManager_h

#include <cocos2d.h>
#include "DialogueWindowConfirm.h"

USING_NS_CC;
using namespace std;

class NoticeManager:public Layer
{
private:
    static NoticeManager* _instance;
    
    cocos2d::Map<string, DialogueWindowConfirm*> _dialogueMap;
protected:
    NoticeManager();
    
public:
    ~NoticeManager();
    
    void put(string key,string title,Color3B titleColor,string content,Color3B contentColor,function<void(Ref*,ui::Widget::TouchEventType)> callback);
    
    DialogueWindowConfirm* get(string key);
    
    //Erase a specific dialogue.
    void erase(string key);
    
    static NoticeManager* getInstance();
};

#endif
