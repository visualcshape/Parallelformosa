//
//  NoticeManager.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/9/4.
//
//

#include "NoticeManager.h"

NoticeManager* NoticeManager::_instance = nullptr;

NoticeManager* NoticeManager::getInstance()
{
    if(_instance==nullptr)
    {
        _instance = new NoticeManager();
        //_instance->autorelease();
    }
    
    return _instance;
}

NoticeManager::NoticeManager()
{
    
}

NoticeManager::~NoticeManager()
{
    if(_instance!=nullptr)
    {
        delete _instance;
        _instance = nullptr;
    }
}

void NoticeManager::put(string key, string title, cocos2d::Color3B titleColor, string content, cocos2d::Color3B contentColor,function<void(Ref*,ui::Widget::TouchEventType)> callback)
{
    DialogueWindowConfirm* pDialogue = DialogueWindowConfirm::create(title, titleColor, content, contentColor);
    
    if(callback!=nullptr)
        pDialogue->addButtonListener(callback);
    
    _dialogueMap.insert(key, pDialogue);
}

DialogueWindowConfirm* NoticeManager::get(string key)
{
    if(_dialogueMap.find(key)!=_dialogueMap.end())
    {
        Map<string, DialogueWindowConfirm*>::const_iterator it = _dialogueMap.find(key);
        
        return it->second;
    }
    CCASSERT(false, "No such key exist.");
    return nullptr;
}

void NoticeManager::erase(string key)
{
    if(_dialogueMap.find(key)!=_dialogueMap.end())
    {
        Map<string, DialogueWindowConfirm*>::const_iterator it = _dialogueMap.find(key);
        _dialogueMap.erase(it);
        return;
    }
    CCASSERT(false, "No such key exist");
}