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

USING_NS_CC;

MapModel* MapModel::mm_pInstance;

MapModel::MapModel(){
	CCLOG("MapModel construct");
	_curPlayer = new PlayerModel();

	_atkPlayer = _defPlayer = _curPlayer;
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
	_curPlayer->init();
	_atkPlayer->init();
	_defPlayer->init();
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

	addBuildingToMap(targetID, _curPlayer->getUID(), mapCoordForPosition(pos, level), level);
}

void MapModel::mapAddBuilding(Building* building){
	_buildings.pushBack((Building*)building);
	if (building->owner == _curPlayer->getUID())
		_curPlayer->addBuilding((Building*)building);
	else if (building->owner == _atkPlayer->getUID())
		_atkPlayer->addBuilding((Building*)building);
	else if (building->owner == _defPlayer->getUID())
		_defPlayer->addBuilding((Building*)building);
}

void MapModel::mapAddTroop(Troop* troop){
	_troops.pushBack((Troop*)troop);
	if (troop->owner == _curPlayer->getUID())
		_curPlayer->addTroop((Troop*)troop);
	else if (troop->owner == _atkPlayer->getUID())
		_atkPlayer->addTroop((Troop*)troop);
	else if (troop->owner == _defPlayer->getUID())
		_defPlayer->addTroop((Troop*)troop);
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

	if (_status == HUD_ID::DEFENSE)
		mapAddBuilding((Building*)target);

	if (_status == HUD_ID::ATTACK)
		mapAddTroop((Troop*)target);
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
bool MapModel::isCoordInsideLayer(MapPoint checkCoordLoc, int level){
	if (level < 0) return false;/*
								if (checkCoordLoc.x < 0) return false;
								if (checkCoordLoc.x >= MAP_X_MAX) return false;
								if (checkCoordLoc.y < 0) return false;
								if (checkCoordLoc.y >= MAP_Y_MAX) return false;*/
	return isTileInsideLayer(tileCoordForMapPoint(checkCoordLoc, level), level);
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
					if (_curPlayer->getUID() == building->owner){
						if (mapName.compare(rm->strWorldMap) == 0){
							writeMapInfo();
							_atkPlayer = _defPlayer = _curPlayer;
							SceneManager::goMapScreen(rm->strPlayerMap[building->owner], HUD_ID::DEFENSE);
						}
						else{
							writeMapInfo();
							_atkPlayer = _defPlayer = _curPlayer;
							SceneManager::goMapScreen(rm->strWorldMap, HUD_ID::DEFENSE);
						}
					}
					else{
						CCLOG(">>>> This is not your building !!! ");
						if (mapName.compare(rm->strWorldMap) == 0){
							writeMapInfo();
							_curPlayer->height = building->height;
							_curPlayer->coord = building->getCoord();
							_atkPlayer = _curPlayer;
							_defPlayer = new PlayerModel();
							_defPlayer->setUID(building->owner);
							CCLOG(">!> _baseBuidling = nullptr => %s", _curPlayer->height == -1 ? "Yes" : "No");
							SceneManager::goMapScreen(rm->strPlayerMap[building->owner], HUD_ID::ATTACK);
						}
						else{
							writeMapInfo();
							_atkPlayer = _defPlayer = _curPlayer;
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
			_background->getContentSize().height);*/

		//if (!backgroundRect.containsPoint(_touchLocation)){
			int isBuildableLevel = canBuildOnTilePosition(_touchLocationInGameLayer);
			if (~isBuildableLevel && _curPlayer->canAddTroop(selID)){
				addBuilding(_touchLocationInGameLayer, isBuildableLevel);
				_curPlayer->consumeResource(selID);
			}
		//}

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

void MapModel::ccdebug(float dt){
	CCLOG("buildings atk:%d def:%d, cur:%d\n", SZ(_atkPlayer->getBuildings()), SZ(_defPlayer->getBuildings()), SZ(_curPlayer->getBuildings()));
	CCLOG("troops atk:%d def:%d, cur:%d\n", SZ(_atkPlayer->getTroops()), SZ(_defPlayer->getTroops()), SZ(_curPlayer->getTroops()));
}
void MapModel::refresh(float dt){
	//@debug modify label.
		{
			char buffer[70];
			sprintf(buffer, "player uid= atk:%d def:%d cur:%d", _atkPlayer->getUID(), _defPlayer->getUID(), _curPlayer->getUID());
			getlblPlayerPos()->setString(buffer);
		}
	//@debug modify label
	{
		char buffer[80];
		sprintf(buffer, "str = %d,\n mag = %d,\n food = %d", _curPlayer->getLstr(), _curPlayer->getGmag(), _curPlayer->getFood());
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
		if (_curPlayer->canAddTroop(i + 1))
			troop->setOpacity(255);
		else
			troop->setOpacity(20);
	}
	//if (pm->_baseBuilding != nullptr)
	//	CCLOG(">>target : height=%d, coord=(%.0f, %.0f)", pm->_baseBuilding->height, pm->_baseBuilding->getCoord().x, pm-//>_baseBuilding->getCoord().y);
}

void MapModel::buildingDelete(Building *target){
	Point pt = tileCoordForMapPoint(target->getCoord(), target->height);
	for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++)
		_pfLayers.at(target->height)->setTileGID(EMPTY_TILE, Point(pt.x + tr, pt.y - tc * 2));
	_defPlayer->removeBuilding(target);
	_buildings.eraseObject(target);
}

void MapModel::troopDelete(Troop *target){
	Point pt = tileCoordForMapPoint(target->getCoord(), target->height);
	for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++)
		_pfLayers.at(target->height)->setTileGID(EMPTY_TILE, Point(pt.x + tr, pt.y - tc * 2));
	_atkPlayer->removeTroop(target);
	_troops.eraseObject(target);
}

bool MapModel::canMoveTo(MapPoint checkMapLoc, int height, int dir, int heightOffset){
	int lr = height;

	if (lr + heightOffset >= 0 && lr + heightOffset < SZ(_pfLayers)){
		checkMapLoc.x += OFFX[dir];
		checkMapLoc.x += OFFY[dir];

		if (!isCoordInsideLayer(checkMapLoc, height + heightOffset)) return false;
		if (!isCoordInsideLayer(checkMapLoc, height - 1 + heightOffset)) return false;

		auto current = _pfLayers.at(lr + heightOffset)->getTileGIDAt(tileCoordForMapPoint(checkMapLoc, lr + heightOffset));
		auto below = _pfLayers.at(lr - 1 + heightOffset)->getTileGIDAt(tileCoordForMapPoint(checkMapLoc, lr - 1 + heightOffset));
		if (below == EMPTY_TILE) return false;
		//CCLOG("belowGID = %d, currentGID = %d\n", below, current);

		if (current == EMPTY_TILE){
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
		}
	}

	return false;
}

PII MapModel::findAttackPath(Troop* troop){
	CCLOG("GOOOOOOOOOOOOOOOO");
	int dp[MAP_MAX_SIZE][MAP_MAX_SIZE][MAP_MAX_SIZE];
	PII rec[MAP_MAX_SIZE][MAP_MAX_SIZE][MAP_MAX_SIZE];
	memset(dp, -1, sizeof(dp));
	for (auto &building : _defPlayer->getBuildings()){
		for (int tr = 0; tr < building->occupy.X; tr++) for (int tc = 0; tc < building->occupy.Y; tc++)
			dp[(int)building->getCoord().x + tr][(int)building->getCoord().y + tc][building->height] = INF;
		//CCLOG("%.0f %.0f %d\n", building->getCoord().x, building->getCoord().y, building->height);
	}
	queue <Vec3> Q;
	Vec3 start = Vec3(troop->getCoord().x, troop->getCoord().y, troop->height);
	Q.push(start);
	dp[(int)start.x][(int)start.y][(int)start.z] = 0;
	//CCLOG("%.0f %.0f %.0f\n", start.x, start.y, start.z);
	while (!Q.empty()){
		Vec3 now = Q.front(); Q.pop();
		for (int k = 0; k < 4; k++)	for (int hofs = 1; hofs >= -1; hofs--){
			Vec3 ntp = now;
			ntp.x += OFFX[k];
			ntp.y += OFFY[k];
			ntp.z = now.z + hofs;
			if (dp[(int)ntp.x][(int)ntp.y][(int)ntp.z] == INF){
				Vec3 backend = now;
				int startdir = NO;
				int starthofs = 0;
				while (backend != start){
					int revdir = rec[(int)backend.x][(int)backend.y][(int)backend.z].X;
					int revhofs = rec[(int)backend.x][(int)backend.y][(int)backend.z].Y;
					backend.x -= OFFX[revdir];
					backend.y -= OFFY[revdir];
					startdir = revdir;
					backend.z -= revhofs;
					starthofs = revhofs;
				}
				return MP(startdir, starthofs);
			}
			//if (!isCoordInsideLayer(MapPoint(ntp.x, ntp.y), ntp.z)) continue;
			if (!isCoordInsideLayer(MapPoint(ntp.x, ntp.y), ntp.z - 1)) continue;
			//auto current = _pfLayers.at(ntp.z)->getTileGIDAt(tileCoordForMapPoint(MapPoint(ntp.x, ntp.y), ntp.z));
			auto below = _pfLayers.at(ntp.z - 1)->getTileGIDAt(tileCoordForMapPoint(MapPoint(ntp.x, ntp.y), ntp.z - 1));
			if (below == EMPTY_TILE) continue;
			Value props = _tileMap->getPropertiesForGID(below);
			if (!props.isNull()){
				ValueMap map = props.asValueMap();
				int type_int = 0;
				if (map.size() == 0)
					type_int = 0;
				else
					type_int = map.at("walkable").asInt();
				if (1 != type_int)
					break;
			}
			//CCASSERT(ntp.z - 1 >= 0, "ntp.z - 1 >= 0");
			if (canMoveTo(MapPoint(now.x, now.y), now.z, k, hofs) && dp[(int)ntp.x][(int)ntp.y][(int)ntp.z] == -1){
				dp[(int)ntp.x][(int)ntp.y][(int)ntp.z] = dp[(int)now.x][(int)now.y][(int)now.z] + 1;
				rec[(int)ntp.x][(int)ntp.y][(int)ntp.z] = MP(k, hofs);
				Q.push(ntp);
				break;
			}
		}
	}
	return MP(NO, 0);
}

void MapModel::commandAttack(){
	_atkPlayer = _curPlayer;
	_curPlayer->commandAttack();
	_defPlayer->commandAttack();
	CCLOG("attack mode activate!!!!");
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
	_atkPlayer->L_str += 1000;
	_atkPlayer->G_mag += 1000;
	}

	//troop->runAction(Sequence::create(RotateTo::create(rotateDuration, cocosAngle), NULL));
	}
	}
	}


	if (_buildings.empty()) //attack win
	SceneManager::goBattleOverScreen("ATTACK WIN!!");
	else if (_troops.empty()){// defense win
	_atkPlayer->height = -1;
	SceneManager::goBattleOverScreen("DEFENSE WIN!!");
	}
	CCLOG("attack mode is now on");*/
}

Building* MapModel::getClosestBuilding(Troop* _troop){
	Building *closestBuilding = nullptr;
	double maxDistance = 999999999.0;

	for (auto &building : _defPlayer->getBuildings()){
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

	for (auto &troop : _atkPlayer->getTroops()){
		for (int tr = 0; tr < _building->occupy.X; tr++) for (int tc = 0; tc < _building->occupy.Y; tc++){
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

//@brief later will modify briefly
void MapModel::troopMove(Troop* _troop, int dir, int heightOffset){
	if (dir == NO) return;
	Point pt = tileCoordForMapPoint(_troop->getCoord(), _troop->height);
	for (int tr = 0; tr < _troop->occupy.X; tr++) for (int tc = 0; tc < _troop->occupy.Y; tc++)
		_pfLayers.at(_troop->height)->setTileGID(EMPTY_TILE, Point(pt.x + tr, pt.y - tc * 2));

	_troop->go(dir, heightOffset);

	pt = tileCoordForMapPoint(_troop->getCoord(), _troop->height);
	for (int tr = 0; tr < _troop->occupy.X; tr++) for (int tc = 0; tc < _troop->occupy.Y; tc++)
		_pfLayers.at(_troop->height)->setTileGID(_troop->id, Point(pt.x + tr, pt.y - tc * 2));
}

//@debug later online
void MapModel::writeMapInfo(){
	ResourceModel *rm = ResourceModel::getModel();
	std::string filename = "MapInfo/" + mapName + ".info";

	FILE *fp = rm->OpenFileW(filename);
	CCASSERT(fp != nullptr, "write map info fail");

	for (auto &building : _buildings)
		fprintf(fp, "%d %.0f %.0f %d %d\n", building->id, building->getCoord().x, building->getCoord().y, building->height, building->owner);
	fclose(fp);
}

void MapModel::readMapInfo(){
	ResourceModel *rm = ResourceModel::getModel();
	std::string filename = "MapInfo/" + mapName + ".info";

	FILE *fp = rm->OpenFileR(filename);
	CCASSERT(fp != nullptr, "read map info fail");

	int id, x, y, height, owner;
	while (~fscanf(fp, "%d %d %d %d %d", &id, &x, &y, &height, &owner)){
		addBuildingToMap(id, owner, MapPoint(x, y), height);
		CCLOG("info: %d %d %d %d %d", id, x, y, height, owner);
	}
	fclose(fp);

	CCLOG(">> _baseBuidling = nullptr => %s", _curPlayer->height == -1 ? "Yes" : "No");
	if (_curPlayer->height != -1 && rm->strWorldMap == mapName){
		CCLOG(">> after win, delete basebuilding");
		//pm->_baseBuilding->sett
		Building* tmp = Building::build(1);
		tmp->height = _curPlayer->height;
		tmp->setCoord(_curPlayer->coord);
		buildingDelete(tmp);
		_curPlayer->height = -1;
	}
	CCLOG(">> _baseBuidling = nullptr => %s", _curPlayer->height == -1 ? "Yes" : "No");
}

void MapModel::go(int dir){
	return;
}