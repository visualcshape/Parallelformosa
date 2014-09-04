#pragma once
#include "cocos2d.h"
#include "GameLayer\BaseLayer.h"

USING_NS_CC;

class MapLayer : public BaseLayer{
public:
	MapLayer();
	~MapLayer();

	virtual bool init();
	virtual void keyPressed(EventKeyboard::KeyCode keyCode, Event *event);

	CREATE_FUNC(MapLayer);

	//@func tilemap
	Point tileCoordForPosition(Point position);
	bool canBuildOnTilePosition(Point pos);

	//@func screen
	Point boundLayerPos(Point newPos);
	void panForTranslation(Point translation);

	//@func building
	void addTower(Point pos);
	void showAllRange(bool visible);

private:
	TMXTiledMap* tileMap;
	TMXLayer* background;
	Node* rangeSprites;
};
