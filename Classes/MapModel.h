#pragma once
#include "cocos2d.h"
#include "Subject.h"

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

	//@func tilemap
	void loadLayers(Vector <TMXLayer*> &tileLayers, std::string prefix);
	Point tileCoordForPosition(Point position);
	Point mapCoordForPosition(Point position, int level);
	int canBuildOnTilePosition(Point pos);
	bool isTileInsideLayer(Point checkTileLoc, int level);

	//@func screen
	Point boundLayerPos(Point newPos);

	//@func building
	void addBuilding(Point pos, int level);
	void showAllRange(bool visible);

	void panForTranslation(Point translation);
	bool outsideBordor(Size contentSize, Point pos);
	void slide(Vec2 translation);

	CC_SYNTHESIZE(TMXTiledMap*, _tileMap, TileMap);
	CC_SYNTHESIZE(Vector <TMXLayer*>, _pfLayers, PFLayers);
	CC_SYNTHESIZE(Node*, _rangeSprites, RangeSprites);

	//@debug
	CC_SYNTHESIZE(Label*, _lblCursorPos, lblCursorPos);
	CC_SYNTHESIZE(Label*, _lblTilePos, lblTilePos);
	CC_SYNTHESIZE(Label*, _lblBuldingPos, lblBuldingPos);

	CC_SYNTHESIZE(Point, _touchOriginLocation, TouchOriginLocation);
	CC_SYNTHESIZE(Point, _touchLocation, TouchLocation);
	CC_SYNTHESIZE(Point, _oldTouchLocation, OldTouchLocation);
	CC_SYNTHESIZE(Point, _touchLocationInGameLayer, TouchLocationInGameLayer);
	CC_SYNTHESIZE(Point, _pressLoc, PressLoc);

	CC_SYNTHESIZE(Sprite*, _background, Background);
	CC_SYNTHESIZE(Sprite*, _selSpriteRange, SelSpriteRange);
	CC_SYNTHESIZE(Sprite*, _selSprite, SelSprite);
	CC_SYNTHESIZE(Node*, _selGroups, SelGroups);

	CC_SYNTHESIZE(Vector <Sprite*>, _movableSprites, MovableSprites);

	CC_SYNTHESIZE(Point, _mapBasePosition, MapBasePosition);
	CC_SYNTHESIZE(Point, _hudBasePosition, HUDBasePosition);
	CC_SYNTHESIZE(Size, _mapContent, MapContent);

	static MapModel* getModel();

protected:
	static MapModel * mm_pInstance;

private:
	std::string mapName;
	int selID;
	bool _prevCursurOutside;
	Vector <Building*> _buildings;
};
