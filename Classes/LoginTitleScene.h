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
#include <CocosGUI.h>
#include <pomelo.h>
#include "CustomLabelMenuItem.h"
#include "Observer.h"
#include "LoginTitleModel.h"
#include "DialogueWindowConfirm.h"
#include "CCPomeloWrapper.h"
#include "json.h"

class LoginTitleScene:public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
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
    cocos2d::MenuItemSprite* m_transparentStartSprite;
    cocos2d::Menu* m_menuForTransparentSpriteMenuItem;
    
    void _doLoginWaterFall();
    //Login waterfall//
    void _connectServer(pc_client_t* client);
    void _sendRequest(pc_client_t* client);
    //authUID callback
    static void _onAuthUIDRequestCallback(pc_request_t* req,int status,json_t* resp);
    ////
    //new login waterfall//
    
    void _newConnectServer();
    void _newSendRequest();
    void _newOnAuthUIDRequestCallback(const CCPomeloRequestResult& result);
    //
    
    static void _startLoading();
    
public:
    MenuLayer();
    
    void settingClickCallback(Ref* pSender);
    
    void aboutClickCallback(Ref* pSender);
    
    void startClickCallback(Ref* pSender);
    
    void loopTouchToStartMenuItemLabel();
    
    CREATE_FUNC(MenuLayer);
};


class InfoLayer:public cocos2d::Layer,public Observer
{
private:
    cocos2d::Label* m_UIDLabel;
    LoginTitleModel* _subject;
public:
    InfoLayer();
    
    ~InfoLayer();
    
    virtual void Update(Subject* changedSubject);
    
    CREATE_FUNC(InfoLayer);
};

#endif
