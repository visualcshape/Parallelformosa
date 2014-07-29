//
//  LoginTitleScene.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/28.
//
//

#ifndef Parallelformosa_Cocos2dx_LoginTitleScene_h
#define Parallelformosa_Cocos2dx_LoginTitleScene_h

#include <cocos2d.h>
#include "CustomLabelMenuItem.h"


class LoginTitleScene:public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void doLogin();
    
    CREATE_FUNC(LoginTitleScene);
    
};

//left this layer black...
class BackgroundLayer:public cocos2d::Layer
{
private:
    cocos2d::Sprite* m_title;
public:
    BackgroundLayer();
    
    CREATE_FUNC(BackgroundLayer);
};

class MenuLayer:public cocos2d::Layer
{
private:
    cocos2d::MenuItemImage* m_settingMenuItemImage;
    cocos2d::MenuItemImage* m_aboutMenuItemImage;
    cocos2d::Label* m_touchToStartLabel;
    CustomMenuItemLabel* m_touchToStartMenuItemLabel;
    cocos2d::Menu* m_menu;
public:
    MenuLayer();
    
    void settingClickCallback(Ref* pSender);
    
    void aboutClickCallback(Ref* pSender);
    
    void startClickCallback(Ref* pSender);
    
    void loopTouchToStartMenuItemLabel();
    
    CREATE_FUNC(MenuLayer);
};


class InfoLayer:public cocos2d::Layer
{
private:
    cocos2d::Label* m_UIDLabel;
public:
    InfoLayer();
    
    CREATE_FUNC(InfoLayer);
};

#endif
