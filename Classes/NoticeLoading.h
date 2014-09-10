//
//  NoticeLoading.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/9/10.
//
//

#ifndef Parallelformosa_Cocos2dx_NoticeLoading_h
#define Parallelformosa_Cocos2dx_NoticeLoading_h

#include <cocos2d.h>
#include <CocosGUI.h>
#include "AppMacro.h"

USING_NS_CC;
using namespace cocos2d::ui;

class NoticeLoading :public Layer
{
private:
    //objs
    Layer* _pBlockLayer;
    ui::Text* _pContentText;
    
    //Data
    std::string _content;
    Color3B _contentColor;
protected:
    NoticeLoading();
public:
    ~NoticeLoading();
    
    static NoticeLoading* create(std::string content,Color3B contentColor);
    
    virtual bool init();
    
    void setContentText(std::string content);
    
    void setContentColor(Color3B contentColor);
    
    CREATE_FUNC(NoticeLoading);
};

#endif
