#include "MapLayer.h"
#include "HUDLayer.h"
#include "MapModel.h"
#include "Building.h"
#include "SceneManager.h"
#include "AppMacro.h"
#include "ResourceModel.h"
#include "PlayerModel.h"
#include "DialogueWindowConfirm.h"
#include "VisibleRect.h"
#include "AppMacro.h"
#include <ctime>
#include "Command.h"
#include "PlayerManager.h"

USING_NS_CC;

MapModel* MapModel::mm_pInstance;

MapModel::MapModel(){
	CCLOG("MapModel construct");
}

MapModel::~MapModel(){
	CCLOG("MapModel destruct");
}

MapModel* MapModel::getModel(){
	if (mm_pInstance == nullptr)
		mm_pInstance = new MapModel();
	return mm_pInstance;
}

void MapModel::init(std::string mapName){
	CCLOG("MapModel init");
	this->mapName = mapName;
	_buildings.clear();
	_troops.clear();
	_pfLayers.clear();
	_movableSprites.clear();
	_tileMap = nullptr;
	_prevCursurOutside = false;
	_status = HUD_ID::DEFENSE;
	PlayerManager::getInstance()->getCurPlayer()->init();
	PlayerManager::getInstance()->getAtkPlayer()->init();
	PlayerManager::getInstance()->getDefPlayer()->init();
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

    string uid = PlayerManager::getInstance()->getCurPlayer()->getUID();
    addBuildingToMap(targetID, uid, mapCoordForPosition(pos, level), level);
}

void MapModel::mapAddBuilding(Building* building){
	CCLOG("MapModel::mapAddBuilding");
	_buildings.pushBack((Building*)building);
	if (building->getOwner() == PlayerManager::getInstance()->getCurPlayer()->getUID())
		PlayerManager::getInstance()->getCurPlayer()->addBuilding((Building*)building);
	else if (building->getOwner() == PlayerManager::getInstance()->getAtkPlayer()->getUID())
		PlayerManager::getInstance()->getAtkPlayer()->addBuilding((Building*)building);
	else if (building->getOwner() == PlayerManager::getInstance()->getDefPlayer()->getUID())
		PlayerManager::getInstance()->getDefPlayer()->addBuilding((Building*)building);
}

void MapModel::mapAddTroop(Troop* troop){
	_troops.pushBack((Troop*)troop);
	if (troop->getOwner() == PlayerManager::getInstance()->getCurPlayer()->getUID())
		PlayerManager::getInstance()->getCurPlayer()->addTroop((Troop*)troop);
	else if (troop->getOwner() == PlayerManager::getInstance()->getAtkPlayer()->getUID())
		PlayerManager::getInstance()->getAtkPlayer()->addTroop((Troop*)troop);
	else if (troop->getOwner() == PlayerManager::getInstance()->getDefPlayer()->getUID())
		PlayerManager::getInstance()->getDefPlayer()->addTroop((Troop*)troop);
}

void MapModel::addBuildingToMap(int ID, std::string& owner, MapPoint buildingLoc, int z){
	TilePoint tileBuildingLoc = tileCoordForMapPoint(buildingLoc, z);

	const int TW = _tileMap->getTileSize().width;
	const int TH = 50;

	PFComponent *target = nullptr;
	/*
	if (_status == HUD_ID::DEFENSE) target = Building::build(ID - 36);
	if (_status == HUD_ID::ATTACK) target = Troop::addTroop(ID - 18);
	*/
	if (ID > 36)
		target = Building::build(ID - 36);
	else if (ID > 18)
		target = Troop::addTroop(ID - 18);
	CCASSERT(target != nullptr, "target != nullptr");

	target->setOwner(owner);

	target->setAnchorPoint(Point(0, 0));
	target->setPosition(Point((buildingLoc.x * TW) + TW / 2, (buildingLoc.y * TH + z * 25 + 75)));
	target->setCoord(buildingLoc);
	target->setZ(z);

	for (int tr = 0; tr < target->getOccupy().X; tr++) for (int tc = 0; tc < target->getOccupy().Y; tc++)
		_pfLayers.at(target->getZ())->setTileGID(target->getID(), Point(tileBuildingLoc.x + tr, tileBuildingLoc.y - tc * 2));

	if (ID > 36)
		mapAddBuilding((Building*)target);
	else if (ID > 18)
		mapAddTroop((Troop*)target);
	/*
	if (_status == HUD_ID::DEFENSE)
	mapAddBuilding((Building*)target);

	if (_status == HUD_ID::ATTACK)
	mapAddTroop((Troop*)target);
	*/
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
			for (int tr = 0; tr < target->getOccupy().X; tr++) for (int tc = 0; tc < target->getOccupy().Y; tc++){
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
		for (int tr = 0; tr < target->getOccupy().X; tr++) for (int tc = 0; tc < target->getOccupy().Y; tc++){
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
		if (couldTileCnt == target->getOccupy().X*target->getOccupy().Y) return lr + 1;
	}
	return -1;
}

bool MapModel::isTileInsideLayer(Point checkTileLoc, int level){
	if (level < 0) return false;
	if (level >= SZ(_pfLayers)) return false;
	if (checkTileLoc.x < 0) return false;
	if (checkTileLoc.x >= _pfLayers.at(level)->getLayerSize().width) return false;
	if (checkTileLoc.y < 0) return false;
	if (checkTileLoc.y >= _pfLayers.at(level)->getLayerSize().height) return false;
	return true;
}

bool MapModel::isCoordInsideLayer(MapPoint checkCoordLoc, int level){
	return isTileInsideLayer(tileCoordForMapPoint(checkCoordLoc, level), level);
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

void MapModel::clickToAddBuildingCursor(int BID){
	ResourceModel *rm = ResourceModel::getModel();

	_selSprite = Sprite::create(rm->strBuilding[BID]);

	_selSpriteRange = Sprite::create(rm->strRangePic);
	_selSpriteRange->setScale(4);

	_selGroups->addChild(_selSpriteRange);
	_selGroups->addChild(_selSprite);
	_selGroups->setPosition(_touchLocation);

	selID = BID;
	showAllRange(true);
}

bool MapModel::tryTouchBegan(){
	/*
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
	}*/
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
    /*!!!
    
	CCLOG("MapModel::tryTouchEnded()");
	ResourceModel *rm = ResourceModel::getModel();

	//@brief just click once
	CCLOG("ccpdist. %.2f\n", _pressLoc.distance(_touchLocation));
	if (_pressLoc.distance(_touchLocation) < 6.5f){
		TilePoint tileLoc = tileCoordForPosition(_touchLocationInGameLayer);
		for (int lr = 5; lr >= 0; lr--){
			for (auto &building : _buildings){
				MapPoint coord = mapCoordForTilePoint(tileLoc, lr);
				if (coord == building->getCoord() && lr == building->getZ()){
					CCLOG(">>@ The building belongs %s", building->getOwner());
					if (PlayerManager::getInstance()->getCurPlayer()->getUID() == building->getOwner()){
						if (mapName.compare(rm->strWorldMap) == 0){
							writeMapInfo();
							PlayerManager::getInstance()->setAtkPlayer(PlayerManager::getInstance()->getCurPlayer());
							PlayerManager::getInstance()->setDefPlayer(PlayerManager::getInstance()->getCurPlayer());
							SceneManager::goMapScreen(rm->strPlayerMap[building->getOwner()], HUD_ID::DEFENSE);
						}
						else{
							writeMapInfo();
							PlayerManager::getInstance()->setAtkPlayer(PlayerManager::getInstance()->getCurPlayer());
							PlayerManager::getInstance()->setDefPlayer(PlayerManager::getInstance()->getCurPlayer());
							SceneManager::goMapScreen(rm->strWorldMap, HUD_ID::DEFENSE);
						}
					}
					else{
						CCLOG(">>>> This is not your building !!! ");
						if (mapName.compare(rm->strWorldMap) == 0){
							writeMapInfo();
							PlayerManager::getInstance()->setAtkPlayer(PlayerManager::getInstance()->getCurPlayer());
							PlayerManager::getInstance()->getCurPlayer()->height = building->getZ();
							PlayerManager::getInstance()->getCurPlayer()->coord = building->getCoord();

							PlayerManager::getInstance()->setDefPlayer(new PlayerModel(building->getOwner()));
							CCLOG(">!> _baseBuidling = nullptr => %s", PlayerManager::getInstance()->getCurPlayer()->height == -1 ? "Yes" : "No");
							SceneManager::goMapScreen(rm->strPlayerMap[building->getOwner()], HUD_ID::ATTACK);
						}
						else{
							writeMapInfo();
							PlayerManager::getInstance()->setAtkPlayer(PlayerManager::getInstance()->getCurPlayer());
							PlayerManager::getInstance()->setDefPlayer(PlayerManager::getInstance()->getCurPlayer());
							SceneManager::goMapScreen(rm->strWorldMap, HUD_ID::DEFENSE);
						}
					}
					return;
				}
			}
		}
	}

	if (_selSprite){
		/*Rect backgroundRect = Rect(_background->getPositionX(),
			_background->getPositionY(),
			_background->getContentSize().width,
			_background->getContentSize().height);

		//if (!backgroundRect.containsPoint(_touchLocation)){
			int isBuildableLevel = canBuildOnTilePosition(_touchLocationInGameLayer);
			if (~isBuildableLevel && PlayerManager::getInstance()->getCurPlayer()->canAddTroop(selID)){
				addBuilding(_touchLocationInGameLayer, isBuildableLevel);
				PlayerManager::getInstance()->getCurPlayer()->consumeResource(selID);
			}
		//}

		_selGroups->removeAllChildren();
		_selSprite = NULL;
		_selSpriteRange = NULL;

		showAllRange(false);
	}
*/
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

void MapModel::ccdebug(float dt){
	CCLOG("buildings atk:%d def:%d, cur:%d\n", SZ(PlayerManager::getInstance()->getAtkPlayer()->getBuildings()), SZ(PlayerManager::getInstance()->getDefPlayer()->getBuildings()), SZ(PlayerManager::getInstance()->getCurPlayer()->getBuildings()));
	CCLOG("troops atk:%d def:%d, cur:%d\n", SZ(PlayerManager::getInstance()->getAtkPlayer()->getTroops()), SZ(PlayerManager::getInstance()->getDefPlayer()->getTroops()), SZ(PlayerManager::getInstance()->getCurPlayer()->getTroops()));
}

void MapModel::produce(float dt){
	PlayerManager::getInstance()->getCurPlayer()->produce(dt);
}

void MapModel::refresh(float dt){
	//@debug modify label.
		{
			char buffer[70];
			sprintf(buffer, "player uid= atk:%s def:%s cur:%s", PlayerManager::getInstance()->getAtkPlayer()->getUID().c_str(), PlayerManager::getInstance()->getDefPlayer()->getUID().c_str(), PlayerManager::getInstance()->getCurPlayer()->getUID().c_str());
			getlblPlayerPos()->setString(buffer);
		}
	//@debug modify label
	{
		char buffer[80];
		sprintf(buffer, "str = %d,\n mag = %d,\n food = %d", PlayerManager::getInstance()->getCurPlayer()->getLstr(), PlayerManager::getInstance()->getCurPlayer()->getGmag(), PlayerManager::getInstance()->getCurPlayer()->getFood());
		getlblResourcePos()->setString(buffer);
	}

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

	for (int i = 0; i < SZ(_movableSprites); i++){
		auto troop = _movableSprites.at(i);
		if (PlayerManager::getInstance()->getCurPlayer()->canAddTroop(i + 1))
			troop->setOpacity(255);
		else
			troop->setOpacity(20);
	}
}

void MapModel::buildingDelete(Building *target){
	CC_ASSERT(target != nullptr);
	setTileGID(target->getCoord(), target->getZ(), target->getOccupy().X, target->getOccupy().Y, EMPTY_TILE);
	PlayerManager::getInstance()->getDefPlayer()->removeBuilding(target);
	_buildings.eraseObject(target);
}

void MapModel::troopDelete(Troop *target){
	CC_ASSERT(target != nullptr);
	setTileGID(target->getCoord(), target->getZ(), target->getOccupy().X, target->getOccupy().Y, EMPTY_TILE);
	PlayerManager::getInstance()->getAtkPlayer()->removeTroop(target);
	_troops.eraseObject(target);
}

bool MapModel::canMoveTo(MapPoint checkMapLoc, int z){
	if (!isCoordInsideLayer(checkMapLoc, z)) return false;
	if (!isCoordInsideLayer(checkMapLoc, z - 1)) return false;

	auto current = getTileGIDAt(checkMapLoc, z);
	auto below = getTileGIDAt(checkMapLoc, z - 1);

	if (current != EMPTY_TILE) return false;
	if (below == EMPTY_TILE) return false;

	Value props = _tileMap->getPropertiesForGID(below);
	if (!props.isNull()){
		ValueMap map = props.asValueMap();
		int type_int = 0;
		if (map.size() == 0)
			type_int = 0;
		else
			type_int = map.at("walkable").asInt();
		if (1 == type_int)
			return true;
	}

	return false;
}

void MapModel::commandAttack(){
	//PlayerManager::getInstance()->getAtkPlayer() = _curPlayer;
	//_curPlayer->commandAttack();
	//PlayerManager::getInstance()->getDefPlayer()->commandAttack();
	//CCLOG("attack mode activate!!!!");
	CMDCountdown::order(BattleModel::getModel())->execute();
}

void MapModel::attackLogic(){
	/*
	auto _tmp_troops = _troops;

	for (auto &troop : _tmp_troops){
	auto target = getClosestBuilding(troop);

	if (target != nullptr){
	bool could_attack = false;
	for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++){
	Point pt = Point(target->getCoord().x + tr, target->getCoord().y + tc);
	Point dist = pt - troop->getCoord();
	if (MAX(abs(dist.x), abs(dist.y)) <= 1) could_attack = true;
	}

	if (!could_attack){
	CCLOG("target = %.0f %.0f\n", target->getCoord().x, target->getCoord().y);

	PII goEvent = findAttackPath(troop);
	int goDir = goEvent.X;
	int gohofs = goEvent.Y;
	if (goDir != NO){
	CCLOG(">> goDir = %d, gohofs = %d\n", goDir, gohofs);
	//if (canMoveTo(troop->getCoord(), troop->height, goDir, gohofs)){
	troopMove(troop, goDir, gohofs);
	//}
	}
	}
	//CCLOG(">>>>>> %.0f %.0f %.0f\n", troop->getCoord().x, troop->getCoord().y, troop->height);

	Point moveVector = troop->getCoord() - target->getCoord();
	if (moveVector.x > 1 && canMoveTo(troop, DIRECTION::LEFT)) troopMove(troop, DIRECTION::LEFT);
	if (moveVector.y > 1 && canMoveTo(troop, DIRECTION::DOWN)) troopMove(troop, DIRECTION::DOWN);
	if (moveVector.x < -1 && canMoveTo(troop, DIRECTION::RIGHT)) troopMove(troop, DIRECTION::RIGHT);
	if (moveVector.y < -1 && canMoveTo(troop, DIRECTION::UP)) troopMove(troop, DIRECTION::UP);

	//float moveAngle = ccpToAngle(moveVector);
	//float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * moveAngle);
	//CCLOG("moveVector = %.2f\n", moveVector.x);
	//float rotateSpeed = 0.5 / M_PI;
	//float rotateDuration = fabs(moveAngle *rotateSpeed);

	if (could_attack){
	//attack building
	CCLOG("troop attack");
	target->hp -= MAX(0, troop->atk - target->def);
	CCLOG("attack : hp = %d, atk = %d, def = %d\n", troop->hp, troop->atk, troop->def);
	CCLOG("defend : hp = %d, atk = %d, def = %d\n", target->hp, target->atk, target->def);

	if (target->hp <= 0){
	buildingDelete(target);
	PlayerManager::getInstance()->getAtkPlayer()->L_str += 1000;
	PlayerManager::getInstance()->getAtkPlayer()->G_mag += 1000;
	}

	//troop->runAction(Sequence::create(RotateTo::create(rotateDuration, cocosAngle), NULL));
	}
	}
	}


	if (_buildings.empty()) //attack win
	SceneManager::goBattleOverScreen("ATTACK WIN!!");
	else if (_troops.empty()){// defense win
	PlayerManager::getInstance()->getAtkPlayer()->height = -1;
	SceneManager::goBattleOverScreen("DEFENSE WIN!!");
	}
	CCLOG("attack mode is now on");*/
}

Building* MapModel::getClosestBuilding(Troop* _troop){
	Building *closestBuilding = nullptr;
	double maxDistance = 999999999.0;

	for (auto &building : PlayerManager::getInstance()->getDefPlayer()->getBuildings()){
		double curDistance = ccpDistance(_troop->getCoord(), building->getCoord());

		if (curDistance < maxDistance){
			closestBuilding = building;
			maxDistance = curDistance;
		}
	}
	return closestBuilding;
}

Troop* MapModel::getClosestTroop(Building* _building){
	Troop *closestTroop = nullptr;
	double maxDistance = 999999999.0;

	for (auto &troop : PlayerManager::getInstance()->getAtkPlayer()->getTroops()){
		for (int tr = 0; tr < _building->getOccupy().X; tr++) for (int tc = 0; tc < _building->getOccupy().Y; tc++){
			Point pt = _building->getCoord();
			double curDistance = ccpDistance(troop->getCoord(), Point(pt.x + tr, pt.y + tc));

			if (curDistance < maxDistance){
				closestTroop = troop;
				maxDistance = curDistance;
			}
		}
	}
	return closestTroop;
}

void MapModel::setTileGID(MapPoint mpt, int z, int xlen, int ylen, int GID){
	Point pt = tileCoordForMapPoint(mpt, z);
	for (int tr = 0; tr < xlen; tr++) for (int tc = 0; tc < ylen; tc++)
		_pfLayers.at(z)->setTileGID(GID, Point(pt.x + tr, pt.y - tc * 2));
}

uint32_t MapModel::getTileGIDAt(MapPoint mpt, int z){
	return _pfLayers.at(z)->getTileGIDAt(tileCoordForMapPoint(mpt, z));
}

//@debug later online
void MapModel::writeMapInfo(bool backup){
	ResourceModel *rm = ResourceModel::getModel();
	string filename = mapName + ".info";

	if (backup) 
		filename += ".backup";

	FILE *fp = rm->OpenFileW(filename);
	CCASSERT(fp != nullptr, "write map info fail");

	for (auto &building : _buildings)
		fprintf(fp, "%d %.0f %.0f %d %s\n", building->getID(), building->getCoord().x, building->getCoord().y, building->getZ(), building->getOwner().c_str());

	if (backup){
		for (auto &troop : _troops)
			fprintf(fp, "%d %.0f %.0f %d %s\n", troop->getID(), troop->getCoord().x, troop->getCoord().y, troop->getZ(), troop->getOwner().c_str());
	}

	fclose(fp);
}

void MapModel::readMapInfo(bool backup){
	ResourceModel *rm = ResourceModel::getModel();
    size_t pos = mapName.find(".tmx");
    mapName.replace(pos, pos+3, "");
	string filename = mapName + ".info";

	if (backup)
		filename += ".backup";

	FILE *fp = rm->OpenFileR(filename);
	CCASSERT(fp != nullptr, "read map info fail");

    int id, x, y, height;
    char owner[256];
	while (~fscanf(fp, "%d %d %d %d %s", &id, &x, &y, &height, owner)){
		CCLOG("info: %d %d %d %d %s", id, x, y, height, owner);
        string ownerString = owner;
		addBuildingToMap(id, ownerString, MapPoint(x, y), height);
	}

	fclose(fp);

	/*
	CCLOG(">> _baseBuidling = nullptr => %s", PlayerManager::getInstance()->getCurPlayer()->height == -1 ? "Yes" : "No");
	if (PlayerManager::getInstance()->getCurPlayer()->height != -1 && rm->strWorldMap == mapName){
		CCLOG(">> after win, delete basebuilding");
		//pm->_baseBuilding->sett
		Building* tmp = Building::build(1);
		tmp->setZ(PlayerManager::getInstance()->getCurPlayer()->height);
		tmp->setCoord(PlayerManager::getInstance()->getCurPlayer()->coord);
		buildingDelete(tmp);
		PlayerManager::getInstance()->getCurPlayer()->height = -1;
	}
	CCLOG(">> _baseBuidling = nullptr => %s", PlayerManager::getInstance()->getCurPlayer()->height == -1 ? "Yes" : "No");
	*/
}
