#pragma once

#include <cocos2d.h>
#include <CocosGUI.h>
#include <pomelo.h>

#include "../Model/LoginTitleModel.h"
#include "BaseLayer.h"
#include "SceneManager.h"

#include "CustomLabelMenuItem.h"
#include "Observer.h"

USING_NS_CC;

class LoginTitleLayer :public BaseLayer{
public:
	LoginTitleLayer();
	~LoginTitleLayer();

	virtual bool init();

	CREATE_FUNC(LoginTitleLayer);
};

class MenuLayer :public BaseLayer{
public:
	MenuLayer();
	~MenuLayer();

	virtual bool init();

	void settingClickCallback(Object* pSender);
	void aboutClickCallback(Object* pSender);
	void startClickCallback(Object* pSender);
	void loopTouchToStartMenuItemLabel();

	CREATE_FUNC(MenuLayer);

private:
    MenuItemImage* m_settingMenuItemImage;
    MenuItemImage* m_aboutMenuItemImage;
    Label* m_touchToStartLabel;
    CustomMenuItemLabel* m_touchToStartMenuItemLabel;
    Menu* m_menu;
	MenuItemSprite* m_transparentStartSprite;
    Menu* m_menuForTransparentSpriteMenuItem;
    
    void _doLoginWaterFall();
    //Login waterfall//
    void _connectServer(pc_client_t* client);
    void _sendRequest(pc_client_t* client);
    //authUID callback
    static void _onAuthUIDRequestCallback(pc_request_t* req,int status,json_t* resp);
    ////
};


class InfoLayer :public BaseLayer, public Observer{
public:
	InfoLayer();
	~InfoLayer();
	virtual void Update(Subject* changedSubject);

	CREATE_FUNC(InfoLayer);

private:
    Label* m_UIDLabel;
    LoginTitleModel* _subject;
};
