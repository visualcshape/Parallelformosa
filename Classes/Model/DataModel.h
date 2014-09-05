#pragma once
#include "cocos2d.h"
#include "Map\Tower.h"
#include "GameLayer\HUDLayer.h"
#include "GameLayer\MapLayer.h"

USING_NS_CC;

class DataModel{
public:
	DataModel();
	~DataModel();

	static DataModel* getModel();

	CC_SYNTHESIZE(MapLayer*, _gameLayer, GameLayer);
	CC_SYNTHESIZE(HUDLayer*, _myHUDLayer, MyHUDLayer);
	CC_SYNTHESIZE(Vector <Tower*>, _towers, Towers);

protected:
	static DataModel * m_pInstance;
};
