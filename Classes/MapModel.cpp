#include "MapLayer.h"
#include "HUDLayer.h"
#include "MapModel.h"
#include "Building.h"
#include "SceneManager.h"
#include "AppMacro.h"
#include "ResourceModel.h"
#include "DialogueWindowConfirm.h"
#include "VisibleRect.h"
#include "AppMacro.h"
#include <ctime>

USING_NS_CC;

MapModel* MapModel::mm_pInstance;

MapModel::MapModel(){
}

MapModel::~MapModel(){
}

MapModel* MapModel::getModel(){
	if (mm_pInstance == nullptr)
		mm_pInstance = new MapModel();
	return mm_pInstance;
}

void MapModel::init(std::string mapName){
	this->mapName = mapName;
	_buildings.clear();
	_troops.clear();
	_pfLayers.clear(); 
	_movableSprites.clear();
	_tileMap = nullptr;
	_player = nullptr;
	_prevCursurOutside = false;
	_status = HUD_ID::DEFENSE;
}

void MapModel::loadLayers(Vector <TMXLayer*> &tileLayers, std::string prefix){
	for (int lr = 0; true; lr++){
		char buffer[30];
		sprintf(buffer, "%d", lr);
		std::string str(prefix);
		str += " ";
		str += std::string(buffer);
		auto tmpLayer = _tileMap->layerNamed(str);
		if (!tmpLayer) break;
		tmpLayer->setAnchorPoint(Point(0, 0));
		tileLayers.pushBack(tmpLayer);
	}
}

TilePoint MapModel::tileCoordForPosition(Point position){
	int x = position.x / _tileMap->getTileSize().width;
	int y = (_tileMap->getMapSize().height *_tileMap->getTileSize().height - position.y) / _tileMap->getTileSize().height;

	return TilePoint(x, y);
}

MapPoint MapModel::mapCoordForPosition(Point position, int level){
	Point tileLoc = tileCoordForPosition(position);

	return mapCoordForTilePoint(tileLoc, level);
}

MapPoint MapModel::mapCoordForTilePoint(TilePoint tileLoc, int level){
	const int MH = _tileMap->getMapSize().height;
	int row = MH - 1 - level - tileLoc.y;
	row = (row) >> 1;

	return MapPoint(tileLoc.x, row);
}

TilePoint MapModel::tileCoordForMapPoint(MapPoint mapLoc, int level){
	const int MH = _tileMap->getMapSize().height;

	return TilePoint(mapLoc.x, (MH - (mapLoc.y + 1) * 2 - level + 1));
}

void MapModel::addBuilding(Point pos, int level){
	if (!~canBuildOnTilePosition(pos)){
		CCLOG("Tile Not Buildable");
		return;
	}
	int targetID = 0;
	if (_status == HUD_ID::DEFENSE) targetID = selID + 36;
	if (_status == HUD_ID::ATTACK) targetID = selID + 18;

	addBuildingToMap(targetID, _player->getUid(), mapCoordForPosition(pos, level), level);
}

void MapModel::addBuildingToMap(int ID, int owner, MapPoint buildingLoc, int level){
	TilePoint tileBuildingLoc = tileCoordForMapPoint(buildingLoc, level);

	const int TW = _tileMap->getTileSize().width;
	const int TH = 50;

	PFComponent *target = nullptr;
	if (_status == HUD_ID::DEFENSE) target = Building::build(ID - 36);
	if (_status == HUD_ID::ATTACK) target = Troop::addTroop(ID - 18);
	CCASSERT(target != nullptr, "target != nullptr");

	target->owner = owner;

	target->setAnchorPoint(Point(0, 0));
	target->setPosition(Point((buildingLoc.x * TW) + TW / 2, (buildingLoc.y * TH + level * 25 + 75)));
	target->setCoord(buildingLoc);
	target->height = level;

	for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++)
		_pfLayers.at(target->height)->setTileGID(target->id, Point(tileBuildingLoc.x + tr, tileBuildingLoc.y - tc * 2));

	if (_status == HUD_ID::DEFENSE) _buildings.pushBack((Building*) target);
	if (_status == HUD_ID::ATTACK) _troops.pushBack((Troop*) target);
	
}
/*@return the highest layer could build, otherwise NONE
 */
int MapModel::canBuildOnTilePosition(Point pos){
	Point TileLoc = tileCoordForPosition(pos);
	
	//@debug modify label.
	{
		char buffer[30];
		sprintf(buffer, "row:%.0f, col:%.0f", TileLoc.x, TileLoc.y);
		getlblTilePos()->setString(buffer);
	}
	
	//@var later need to Resoucre Manager
	PFComponent *target = nullptr;
	if (_status == HUD_ID::DEFENSE) target = Building::build(selID);
	if (_status == HUD_ID::ATTACK) target = Troop::addTroop(selID);
	CCASSERT(target != nullptr, "target != nullptr");
	for (int lr = SZ(_pfLayers) - 1; lr >= 0; lr--){
		Point buildingLoc = mapCoordForPosition(pos, lr);

		//@procedure check for no tiles on the tile.
		bool noTileOnDirectly = true;
		if (lr < SZ(_pfLayers) - 1){
			for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++){
				for (int offset = 0; offset <= 1; offset++){
					Point checkTileLoc = Point(TileLoc.x + tr, TileLoc.y - tc * 2 + offset);
					if (isTileInsideLayer(checkTileLoc, lr)){
						int tileGid = _pfLayers.at(lr + 1)->getTileGIDAt(checkTileLoc);
						if (tileGid != EMPTY_TILE) noTileOnDirectly = false;
					}
				}
			}
		}
		if (!noTileOnDirectly) continue;

		int couldTileCnt = 0;
		for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++){
			for (int offset = 1; offset <= 2; offset++){
				Point checkTileLoc = Point(TileLoc.x + tr, TileLoc.y - tc * 2 + offset);
				if (isTileInsideLayer(checkTileLoc, lr)){
					int tileGid = _pfLayers.at(lr)->getTileGIDAt(checkTileLoc);

					Value props = _tileMap->getPropertiesForGID(tileGid);
					if (!props.isNull()){
						ValueMap map = props.asValueMap();
						int type_int = 0;
						if (map.size() == 0)
							type_int = 0;
						else
							type_int = map.at("buildable").asInt();
						if (1 == type_int){
							couldTileCnt++;
							break;
						}
					}
				}
			}
		}
		if (couldTileCnt == target->occupy.X*target->occupy.Y) return lr + 1;
	}
	return -1;
}

bool MapModel::isTileInsideLayer(Point checkTileLoc, int level){
	if (checkTileLoc.x < 0) return false;
	if (checkTileLoc.x >= _pfLayers.at(level)->getLayerSize().width) return false;
	if (checkTileLoc.y < 0) return false;
	if (checkTileLoc.y >= _pfLayers.at(level)->getLayerSize().height) return false;
	return true;
}

Point MapModel::boundLayerPos(Point newPos){
	Size winSize = Director::getInstance()->getWinSize();
	Point retval = newPos;

	// inside the map, don't over border.
	retval.x = MIN(retval.x, 0);
	retval.x = MAX(retval.x, -_pfLayers.at(0)->getContentSize().width + winSize.width);

	retval.y = MIN(retval.y, 0);
	retval.y = MAX(retval.y, -_pfLayers.at(0)->getContentSize().height + winSize.height);

	return retval;
}

void MapModel::showAllRange(bool visible){
	_rangeSprites->removeAllChildren();
	if (visible){
		for (auto &building : _buildings){
			ResourceModel *rm = ResourceModel::getModel();
			auto rangeSprite = Sprite::create(rm->strRangePic);
			rangeSprite->setScale(4);
			rangeSprite->setPosition(building->getPosition());
			_rangeSprites->addChild(rangeSprite);
		}
	}
}

bool MapModel::tryTouchBegan(){
	//@debug modify label.
	char buffer[70];
	sprintf(buffer, "palyer uid=%d", _player->getUid());
	getlblPlayerPos()->setString(buffer);

	Sprite * newSprite = NULL;
	for (int i = 0; i < _movableSprites.size(); i++){
		Sprite* sprite = _movableSprites.at(i);
		Rect pos_rect = Rect((sprite->getPositionX() - sprite->getContentSize().width / 2),
			(sprite->getPositionY() - sprite->getContentSize().height / 2),
			sprite->getContentSize().width,
			sprite->getContentSize().height);
		float xMin = pos_rect.getMinX();
		float xMax = pos_rect.getMaxX();
		float yMIn = pos_rect.getMinY();
		float yMax = pos_rect.getMaxY();
		if (pos_rect.containsPoint(_touchLocation)){
			ResourceModel *rm = ResourceModel::getModel();
			_selSpriteRange = Sprite::create(rm->strRangePic);
			_selSpriteRange->setScale(4);
			_selGroups->setPosition(sprite->getPosition());
			_selGroups->addChild(_selSpriteRange);

			newSprite = Sprite::createWithTexture(sprite->getTexture()); //sprite;
			_selSprite = newSprite;
			_selGroups->addChild(newSprite);

			selID = i + 1;
			showAllRange(true);
		}
	}
	return true;
}

void MapModel::tryTouchMoved(){
	Point translation = _touchLocation - _oldTouchLocation;

	if (_prevCursurOutside){
		_selGroups->setPosition(_touchLocation);
		_prevCursurOutside = false;
	}

	if (_selSprite){
		_selGroups->setPosition(_selGroups->getPosition() + translation);

		//@debug modify label.
		char buffer[30];
		sprintf(buffer, "x:%.1f, y:%.1f", _touchLocationInGameLayer.x, _touchLocationInGameLayer.y);
		getlblCursorPos()->setString(buffer);

		int isBuildableLevel = canBuildOnTilePosition(_touchLocationInGameLayer);
		if (~isBuildableLevel)
			_selSprite->setOpacity(200);
		else
			_selSprite->setOpacity(50);
	}
	else{
		slide(translation);
	}
}

void MapModel::tryTouchEnded(){
	ResourceModel *rm = ResourceModel::getModel();

	//@brief just click once
	CCLOG("ccpdist. %.2f\n", _pressLoc.distance(_touchLocation));
	if (_pressLoc.distance(_touchLocation) < 6.5f){
		TilePoint tileLoc = tileCoordForPosition(_touchLocationInGameLayer);
		for (int lr = 5; lr >= 0; lr--){
			for (auto &building : _buildings){
				MapPoint coord = mapCoordForTilePoint(tileLoc, lr);
				if (coord == building->getCoord() && lr == building->height){
					CCLOG(">>@ The building belongs %d", building->owner);
					if (_player->getUid() == building->owner){
						if (mapName.compare(rm->strWorldMap) == 0){
							writeMapInfo();
							SceneManager::goMapScreen(rm->strPlayerMap[_player->getUid()], HUD_ID::DEFENSE);
						}
						else{
							writeMapInfo();
							SceneManager::goMapScreen(rm->strWorldMap, HUD_ID::DEFENSE);
						}
					}
					else{
						CCLOG(">>>> This is not your building !!! ");
						if (mapName.compare(rm->strWorldMap) == 0){
							writeMapInfo();
							SceneManager::goMapScreen(rm->strPlayerMap[building->owner], HUD_ID::ATTACK);
						}
						else{
							writeMapInfo();
							SceneManager::goMapScreen(rm->strWorldMap, HUD_ID::DEFENSE);
						}
					}
					return;
				}
			}
		}
	}

	if (_selSprite){
		Rect backgroundRect = Rect(_background->getPositionX(),
			_background->getPositionY(),
			_background->getContentSize().width,
			_background->getContentSize().height);

		if (!backgroundRect.containsPoint(_touchLocation)){
			int isBuildableLevel = canBuildOnTilePosition(_touchLocationInGameLayer);
			if (~isBuildableLevel)
				addBuilding(_touchLocationInGameLayer, isBuildableLevel);
		}

		_selGroups->removeAllChildren();
		_selSprite = NULL;
		_selSpriteRange = NULL;

		showAllRange(false);
	}
}

bool MapModel::outsideBordor(Size contentSize, Point pos){
	//@remind use BORDER_PIXEL pixel to avoid border error
	_prevCursurOutside = false;
	if (pos.x < BORDER_PIXEL || pos.x + BORDER_PIXEL > contentSize.width)
		_prevCursurOutside = true;
	if (pos.y < BORDER_PIXEL || pos.y + BORDER_PIXEL > contentSize.height)
		_prevCursurOutside = true;
	return _prevCursurOutside;
}

void MapModel::slide(Vec2 translation){
	Point oldMapBasePosition = _mapBasePosition;
	_mapBasePosition = boundLayerPos(_mapBasePosition + translation);
	Point HUDtranstion = _mapBasePosition - oldMapBasePosition;
	_hudBasePosition = _hudBasePosition - HUDtranstion;
	Notify();
}

void MapModel::refresh(float dt){
	if (_selSprite){
		if (_selGroups->getPositionX() < BORDER_PIXEL)
			slide(Vec2(SLIDE_RATE, 0));
		if (_selGroups->getPositionX() >= _mapContent.width - BORDER_PIXEL)
			slide(Vec2(-SLIDE_RATE, 0));
		if (_selGroups->getPositionY() < BORDER_PIXEL)
			slide(Vec2(0, SLIDE_RATE));
		if (_selGroups->getPositionY() >= _mapContent.height - BORDER_PIXEL)
			slide(Vec2(0, -SLIDE_RATE));
	}
}

void MapModel::attackLogic(float dt){
	for (auto &troop : _troops){
		auto target = getClosestBuilding(troop);

		if (target != nullptr){
			Point moveVector = troop->getCoord() - target->getCoord();
			//float moveAngle = ccpToAngle(moveVector);
			//float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * moveAngle);
			CCLOG("moveVector = %.2f\n", moveVector.x);
			//float rotateSpeed = 0.5 / M_PI;
			//float rotateDuration = fabs(moveAngle *rotateSpeed);
			if (moveVector.x > 1) troopMove(troop, 0);
			if (moveVector.y > 1) troopMove(troop, 1);
			if (moveVector.x < -1) troopMove(troop, 2);
			if (moveVector.y < -1) troopMove(troop, 3);
			//troop->runAction(Sequence::create(RotateTo::create(rotateDuration, cocosAngle), NULL));
		}
	}
}

Building* MapModel::getClosestBuilding(Troop* _troop){
	Building *closestBuilding = nullptr;
	double maxDistance = 999999999.0;

	for (auto &building : _buildings){
		double curDistance = ccpDistance(_troop->getPosition(), building->getPosition());

		if (curDistance < maxDistance){
			closestBuilding = building;
			maxDistance = curDistance;
		}
	}
	return closestBuilding;
}

//@brief later will modify briefly
void MapModel::troopMove(Troop* _troop, int direction){
	Point pt = tileCoordForMapPoint(_troop->getCoord(), _troop->height);
	for (int tr = 0; tr < _troop->occupy.X; tr++) for (int tc = 0; tc < _troop->occupy.Y; tc++)
		_pfLayers.at(_troop->height)->setTileGID(EMPTY_TILE, Point(pt.x + tr, pt.y - tc * 2));
	switch (direction){
	case 0:	_troop->goLeft(); break;
	case 1: _troop->goDown(); break;
	case 2: _troop->goRight(); break;
	case 3: _troop->goUp(); break;
	default: CCASSERT(false, "direction must >= 0 & <= 3");
	}
	pt = tileCoordForMapPoint(_troop->getCoord(), _troop->height);
	for (int tr = 0; tr < _troop->occupy.X; tr++) for (int tc = 0; tc < _troop->occupy.Y; tc++)
		_pfLayers.at(_troop->height)->setTileGID(_troop->id, Point(pt.x + tr, pt.y - tc * 2));
}
//@debug later online
void MapModel::writeMapInfo(){
	std::string path = "MapInfo/" + mapName + ".info";
	std::string filepath = CCFileUtils::getInstance()->fullPathForFilename(path.c_str());	
	FILE *fp = fopen(filepath.c_str(), "w");
	CCASSERT(fp != nullptr, "write map info fail");
	for (auto &building : _buildings)
		fprintf(fp, "%d %.0f %.0f %d %d\n", building->id, building->getCoord().x, building->getCoord().y, building->height, building->owner);
	fclose(fp);
}

void MapModel::readMapInfo(){
	std::string path = "MapInfo/" + mapName + ".info";
	std::string filepath = CCFileUtils::getInstance()->fullPathForFilename(path.c_str());
	CCLOG(filepath.c_str());
	FILE *fp = fopen(filepath.c_str(), "r");

	//@brief create empty file
	if (fp == nullptr){
		fp = fopen(filepath.c_str(), "w");
		fclose(fp);
		fp = fopen(filepath.c_str(), "r");
	}

	CCASSERT(fp != nullptr, "read map info fail");

	int id, x, y, height, owner;
	while (~fscanf(fp, "%d %d %d %d %d", &id, &x, &y, &height, &owner)){
		addBuildingToMap(id, owner, MapPoint(x, y), height);
		CCLOG("info: %d %d %d %d %d", id, x, y, height, owner);
	}
	fclose(fp);
}
