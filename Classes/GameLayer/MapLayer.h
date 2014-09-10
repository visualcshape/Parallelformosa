#pragma once
#include "cocos2d.h"
#include "GameLayer\BaseLayer.h"
#include "Map\Building.h"

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
	int canBuildOnTilePosition(Point pos, int selID);
	Point mapCoordForPosition(Point position, int level);

	//@func screen
	Point boundLayerPos(Point newPos);
	void panForTranslation(Point translation);
	
	//@func building
	void addBuilding(Point pos, int selID, int level);
	void showAllRange(bool visible);

	//@func tilemark
	void setTileMark(Point pos, int selID, int level, bool canBuild);
	void removeTileMark();

private:
	TMXTiledMap* tileMap;
	Vector <TMXLayer*> tileLayers;
	Node* rangeSprites;
	Node* tileMark;
};
