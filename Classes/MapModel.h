#pragma once
#include "cocos2d.h"
#include "Building.h"
#include "Subject.h"
#include "Troop.h"
#include "CMD.h"

USING_NS_CC;
using namespace std;

class MapModel : public Layer,public Subject{
public:
	static const int EMPTY_TILE = 54;

	static const int GREEN_MARK = 55;
	static const int RED_MARK = 56;
	static const int EMPTY_MARK = 57;

	const float SLIDE_RATE = 25.0f;
	const float REFRESH_RATE = 0.03f;
	const float PRODUCE_RATE = 1.0f;
	static const int BORDER_PIXEL = 40;
	static const int BAR_ICON = 5;
    
	static const int TW = 101;
	static const int TH = 40;
    //enum
    typedef enum Weather
    {
        NONEWEATHER,
        SUNNY,
        RAIN
    }Weather;

    //enum changed data
    typedef enum ChangedData
    {
        NONE=-1
    }ChangedData;
public:
	MapModel();
	~MapModel();

	void init(std::string mapName);

	bool tryTouchBegan();
	void tryTouchMoved();
	void tryTouchEnded();

	void refresh(float dt);
	void ccdebug(float dt);

	void clickToAddBuildingCursor(int BID);
	
	//@func tilemap
	void loadLayers(Vector <TMXLayer*> &tileLayers, string prefix);
	int canBuildOnTilePosition(Point pos); 
	//int canWalkOnTilePosition(MapPoint checkMapLoc);
	bool isTileInsideLayer(MapPoint checkTileLoc, int level);
	bool isCoordInsideLayer(MapPoint checkMapLoc, int level);

	//@func screen
	Point boundLayerPos(Point newPos);
	bool outsideBordor(Size contentSize, Point pos);
	void slide(Vec2 translation);

	//@func building
	void addBuilding(Point pos, int level);
	void showAllRange(bool visible);

	//@func sync to database (now used update locally)
	void readMapInfo(bool backup = false);
	void writeMapInfo(bool backup = false);

	bool canMoveTo(MapPoint checkMapLoc, int z);
	Building* getClosestBuilding(Troop* _troop);
	Troop* getClosestTroop(Building* _building);
	void buildingDelete(Building *_building);
	void troopDelete(Troop *_troop);

	void mapAddBuilding(Building* building);
	void mapAddTroop(Troop* troop);

	CC_SYNTHESIZE(TMXTiledMap*, _tileMap, TileMap);
	CC_SYNTHESIZE(Vector <TMXLayer*>, _pfLayers, PFLayers);
	CC_SYNTHESIZE(Node*, _rangeSprites, RangeSprites);

	//@debug
	CC_SYNTHESIZE(Label*, _lblCursorPos, lblCursorPos);
	CC_SYNTHESIZE(Label*, _lblTilePos, lblTilePos);
	CC_SYNTHESIZE(Label*, _lblPlayerPos, lblPlayerPos);
	CC_SYNTHESIZE(Label*, _lblCountdownPos, lblCountdownPos);

	//@brief touch event point
	CC_SYNTHESIZE(Point, _touchOriginLocation, TouchOriginLocation);
	CC_SYNTHESIZE(Point, _touchLocation, TouchLocation);
	CC_SYNTHESIZE(Point, _oldTouchLocation, OldTouchLocation);
	CC_SYNTHESIZE(Point, _touchLocationInGameLayer, TouchLocationInGameLayer);
	CC_SYNTHESIZE(Point, _pressLoc, PressLoc);

	//@brief layer view element
	CC_SYNTHESIZE(Sprite*, _background, Background);
	CC_SYNTHESIZE(Sprite*, _selSpriteRange, SelSpriteRange);
	CC_SYNTHESIZE(Sprite*, _selSprite, SelSprite);
	CC_SYNTHESIZE(Node*, _selGroups, SelGroups);

	CC_SYNTHESIZE(Vector <Sprite*>, _movableSprites, MovableSprites);

	CC_SYNTHESIZE(Point, _mapBasePosition, MapBasePosition);
	CC_SYNTHESIZE(Point, _hudBasePosition, HUDBasePosition);
	CC_SYNTHESIZE(Size, _mapContent, MapContent);

	CC_SYNTHESIZE(HUD_ID, _status, Status);

	static MapModel* getModel();

	//@func tilemap of coord transformation system.
	TilePoint tileCoordForPosition(Point position);
	MapPoint mapCoordForPosition(Point position, int z);
	MapPoint mapCoordForTilePoint(TilePoint tileLoc, int z);
	TilePoint tileCoordForMapPoint(MapPoint mapLoc, int z);

	void setTileGID(MapPoint mpt, int z, int xlen, int ylen, int GID);
	uint32_t getTileGIDAt(MapPoint mpt, int z);

    CC_SYNTHESIZE(string, mapName, MapName);
    
    //////////////////
    Weather getWeather();
    
    void setWeather(Weather weather);
    
    ChangedData getChangedData();

protected:
	static MapModel * mm_pInstance;

	//@func internally add building
	void addBuildingToMap(int ID, string& owner, MapPoint pos, int z, bool writeToServer = true);

private:
	int selID;
	bool _prevCursurOutside; 
	Vector <Building*> _buildings;
	Vector <Troop*> _troops;
    
    Weather _weather;
    ChangedData _changedData;
};
