#pragma once
#include "cocos2d.h"
#include "Building.h"
#include "Subject.h"
#include "Troop.h"
#include "PlayerModel.h"

USING_NS_CC;

class MapModel : public Ref, public Subject{
public:
	static const int EMPTY_TILE = 54;

	static const int GREEN_MARK = 55;
	static const int RED_MARK = 56;
	static const int EMPTY_MARK = 57;

	const float SLIDE_RATE = 25.0f;
	const float REFRESH_RATE = 0.03f;
	static const int BORDER_PIXEL = 40;
	static const int BAR_ICON = 5;

public:
	MapModel();
	~MapModel();

	void init(std::string mapName);

	bool tryTouchBegan();
	void tryTouchMoved();
	void tryTouchEnded();
	void refresh(float dt);
	void ccdebug(float dt);
	void attackLogic();
	void commandAttack();

	//@func tilemap
	void loadLayers(Vector <TMXLayer*> &tileLayers, std::string prefix);
	int canBuildOnTilePosition(Point pos); 
	//int canWalkOnTilePosition(MapPoint checkMapLoc);
	bool isTileInsideLayer(Point checkTileLoc, int level);
	bool isCoordInsideLayer(Point checkTileLoc, int level);
	PII findAttackPath(Troop* troop);

	//@func screen
	Point boundLayerPos(Point newPos);
	bool outsideBordor(Size contentSize, Point pos);
	void slide(Vec2 translation);

	//@func building
	void addBuilding(Point pos, int level);
	void showAllRange(bool visible);

	//@func sync to database (now used update locally)
	void readMapInfo();
	void writeMapInfo();

	bool canMoveTo(MapPoint checkMapLoc, int height, int dir, int heightOffset);
	void troopMove(Troop* _troop, int dir, int heightOffset);
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
	CC_SYNTHESIZE(Label*, _lblResourcePos, lblResourcePos);

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

	CC_SYNTHESIZE(PlayerModel*, _curPlayer, CurPlayer);
	CC_SYNTHESIZE(PlayerModel*, _atkPlayer, AtkPlayer);
	CC_SYNTHESIZE(PlayerModel*, _defPlayer, DefPlayer);

	static MapModel* getModel();

protected:
	static MapModel * mm_pInstance;

	//@func tilemap of coord transformation system.
	TilePoint tileCoordForPosition(Point position);
	MapPoint mapCoordForPosition(Point position, int level);
	MapPoint mapCoordForTilePoint(TilePoint tileLoc, int level);
	TilePoint tileCoordForMapPoint(MapPoint mapLoc, int level);

	//@func internally add building
	void addBuildingToMap(int ID, int owner, MapPoint pos, int level);
	void go(int dir);

private:
	std::string mapName;
	int selID;
	bool _prevCursurOutside; 
	Vector <Building*> _buildings;
	Vector <Troop*> _troops;
};
