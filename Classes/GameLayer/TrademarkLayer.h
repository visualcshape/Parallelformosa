#pragma once
#include "cocos2d.h"
#include "SceneManager.h"
#include "GameLayer\BaseLayer.h"

USING_NS_CC;

class TrademarkLayer :public BaseLayer{
public:
	TrademarkLayer();
	~TrademarkLayer();

	virtual bool init();

	void animate();
	void back(Object* pSender);

	CREATE_FUNC(TrademarkLayer);

protected:
	void animateTrademark();

private:
	Sprite* m_tradeMark;
	MenuItemSprite* m_transparentSpriteMenuItem;
	Menu* m_menu;
};
