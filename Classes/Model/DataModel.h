#pragma once
#include "cocos2d.h"
#include "Map\Building.h"
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
	CC_SYNTHESIZE(Vector <Building*>, _building, Buildings);

protected:
	static DataModel * dm_pInstance;
};
