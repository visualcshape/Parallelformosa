#pragma once
#include "cocos2d.h"
#include "HUDLayer.h"
#include "MapLayer.h"
#include "Building.h"

USING_NS_CC;

class DataModel{
public:
	DataModel();
	~DataModel();

	static DataModel* getModel();

	CC_SYNTHESIZE(MapLayer*, _gameLayer, GameLayer);
	//CC_SYNTHESIZE(HUDLayer*, _myHUDLayer, MyHUDLayer);
	CC_SYNTHESIZE(std::string, _MapName, MapName);

protected:
	static DataModel * dm_pInstance;
};
