#pragma once
#include "cocos2d.h"
#include "GameLayer\BaseLayer.h"

USING_NS_CC;

class MapLayer : public BaseLayer{
public:
	MapLayer();
	~MapLayer();

	virtual bool init();

	CREATE_FUNC(MapLayer);

	Point tileCoordForPosition(Point position);
	bool canBuildOnTilePosition(Point pos);
	void addTower(Point pos);
	Point boundLayerPos(Point newPos);

private:
	TMXTiledMap* tileMap;
	TMXLayer* background;
};