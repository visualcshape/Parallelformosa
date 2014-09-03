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
	virtual void keyReleased(EventKeyboard::KeyCode keyCode, Event *event);

	CREATE_FUNC(MapLayer);

	Point tileCoordForPosition(Point position);
	bool canBuildOnTilePosition(Point pos);
	void addTower(Point pos);
	Point boundLayerPos(Point newPos);
	void panForTranslation(Point translation);

private:
	TMXTiledMap* tileMap;
	TMXLayer* background;
};