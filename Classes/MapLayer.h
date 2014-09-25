#pragma once
#include "cocos2d.h"
#include "BaseLayer.h"
#include "Building.h"

USING_NS_CC;

class MapLayer : public BaseLayer{
	static const int EMPTY_TILE = 54;
	static const int GREEN_MARK = 55;
	static const int RED_MARK = 56;
	static const int EMPTY_MARK = 57;

public:
	MapLayer();
	~MapLayer();

	virtual bool init();
	virtual void keyPressed(EventKeyboard::KeyCode keyCode, Event *event);

	CREATE_FUNC(MapLayer);

	//@func tilemap
	Point tileCoordForPosition(Point position);
	Point mapCoordForPosition(Point position, int level);
	int canBuildOnTilePosition(Point pos, int selID);
	bool isTileInsideLayer(Point checkTileLoc, int level);

	//@func screen
	Point boundLayerPos(Point newPos);
	void panForTranslation(Point translation);
	
	//@func building
	void addBuilding(Point pos, int selID, int level);
	void showAllRange(bool visible);

	//@func tilemark
	void setTileMark(Point pos, int selID, int level, bool canBuild);
	void removeTileMark();

protected:
	void loadLayers(Vector <TMXLayer*> *tileLayers, std::string prefix);

private:
	TMXTiledMap* tileMap;
	Vector <TMXLayer*> PFLayers;

	Node* rangeSprites;
	std::vector <Vec3> tileMark;
};
