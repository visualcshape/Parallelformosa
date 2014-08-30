#pragma once
#include "cocos2d.h"
#include "Map\Tower.h"
#include "GameLayer\HUDLayer.h"
#include "GameLayer\MapLayer.h"

USING_NS_CC;

class DataModel{
public:
	MapLayer* _gameLayer;
	HUDLayer* _HUDLayerLayer;
	Vector <Tower*> towers;
	static DataModel* getModel();
private:
	DataModel(){};
	static DataModel * m_pInstance;
};