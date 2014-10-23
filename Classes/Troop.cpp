#include "ResourceModel.h"
#include "MapModel.h"
#include "Troop.h"
#include "Command.h"
#include "PlayerManager.h"

Troop::Troop(){
}

Troop::~Troop(){
	_moves.clear();
	_states.clear();
}

bool Troop::init(){
	if (!PFComponent::init())
		return false;
	
	_target = nullptr;
	_oid = -1;
	return true;
}

Troop* Troop::addTroop(int TID){
	auto unitMap = UnitTypeModel::getInstance()->getUnitTypeMap();
	map<string, UnitType>::const_iterator itr;
	for (itr = unitMap->begin(); itr != unitMap->end(); itr++)
	{
		if (itr->second.id == TID)
			break;
	}
	CC_ASSERT(itr != unitMap->end());
	Troop* troop = Troop::create();
	troop->_sprite = Sprite::create(itr->second.resourcePath);
	troop->addChild(troop->_sprite, 0);
	troop->_id = itr->second.gid;
	troop->_occupy = MP(1, 1);
	troop->_atk = itr->second.atk;
	troop->_def = itr->second.def;
	troop->_hp = itr->second.baseHP;

	return troop;

	//Deprecated...
	/*
	CCASSERT(TID >= 1 && TID <= 5, "TID index outside");
	ResourceModel *rm = ResourceModel::getModel();

	Troop* troop = Troop::create();
	troop->_sprite = Sprite::create(rm->strTroop[TID]);
	troop->addChild(troop->_sprite, 0);
	troop->_id = rm->GIDTroop[TID];
	troop->_occupy = MP(1, 1);

	troop->_atk = 30;
	troop->_def = 15;
	troop->_hp = 100;

	return troop;*/
}

void Troop::attackLogic(){
	MapModel *mm = MapModel::getModel();
	if (isDead()){
		mm->troopDelete(this);
		return;
	}

	if (_target == nullptr)
		_moves.clear();

	if (_moves.empty()){
		_target = nullptr;
		if (!buildAttackPath())
			return;
	}

	if (!_moves.empty() && _moves.back()->execute())
		_moves.popBack();
	if (!_states.empty() && _states.back()->execute())
		_states.popBack();

	tryToAttack();
	/*
	auto target = mm->getClosestBuilding(this);

	if (target != nullptr){
		bool could_attack = false;
		for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++){
			Point pt = Point(target->getCoord().x + tr, target->getCoord().y + tc);
			Point dist = pt - getCoord();
			if (MAX(abs(dist.x), abs(dist.y)) <= 1) could_attack = true;
		}
		
		if (!could_attack){
			CCLOG("target = %.0f %.0f\n", target->getCoord().x, target->getCoord().y);

			PII goEvent = mm->findAttackPath(this);
			int goDir = goEvent.X;
			int gohofs = goEvent.Y;
			if (goDir != NO){
				CCLOG(">> goDir = %d, gohofs = %d\n", goDir, gohofs);
				//if (canMoveTo(troop->getCoord(), troop->height, goDir, gohofs)){
				CMDMove::order(this, goDir, gohofs)->execute();
				//}
			}
			else{
				Point moveVector = getCoord() - target->getCoord();
				for (int hofs = -1; hofs <= 1; hofs++){
					if (moveVector.x > 1 && mm->canMoveTo(getCoord(), height, LEFT, hofs)){
						mm->troopMove(this, LEFT, hofs);
						break;
					}
					if (moveVector.y > 1 && mm->canMoveTo(getCoord(), height, DOWN, hofs)){
						mm->troopMove(this, DOWN, hofs);
						break;
					}
					if (moveVector.x < -1 && mm->canMoveTo(getCoord(), height, RIGHT, hofs)){
						mm->troopMove(this, RIGHT, hofs);
						break;
					}
					if (moveVector.y < -1 && mm->canMoveTo(getCoord(), height, UP, hofs)){
						mm->troopMove(this, UP, hofs);
						break;
					}
				}
			}
		}
		//CCLOG(">>>>>> %.0f %.0f %.0f\n", troop->getCoord().x, troop->getCoord().y, troop->height);
		
		//float moveAngle = ccpToAngle(moveVector);
		//float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * moveAngle);
		//CCLOG("moveVector = %.2f\n", moveVector.x);
		//float rotateSpeed = 0.5 / M_PI;
		//float rotateDuration = fabs(moveAngle *rotateSpeed);

		if (could_attack){
			//attack building
			CCLOG("troop attack");
			target->hp -= MAX(0, atk - target->def);
			CCLOG("attack : hp = %d, atk = %d, def = %d\n", hp, atk, def);
			CCLOG("defend : hp = %d, atk = %d, def = %d\n", target->hp, target->atk, target->def);

			if (target->hp <= 0){
				mm->buildingDelete(target);
				mm->getAtkPlayer()->gainLstr(1000);
				mm->getAtkPlayer()->gainGmag(1000);
			}

			//troop->runAction(Sequence::create(RotateTo::create(rotateDuration, cocosAngle), NULL));
		}
	}*/
}
void Troop::replayLogic(){
	MapModel *mm = MapModel::getModel();
	if (isDead()){
		mm->troopDelete(this);
		return;
	}
	if (!_moves.empty() && _moves.front()->execute())
		_moves.erase(_moves.begin());
	if (!_states.empty() && _states.front()->execute())
		_states.erase(_states.begin());
}

bool Troop::buildAttackPath(){
	CCLOG("Troop::buildAttackPath()");
	CC_ASSERT(PlayerManager::getInstance()->getDefPlayer() != nullptr);
	if (SZ(PlayerManager::getInstance()->getDefPlayer()->getBuildings()) == 0)
		return false;

	int dp[MAP_MAX_SIZE][MAP_MAX_SIZE][MAP_MAX_SIZE];
	PII rec[MAP_MAX_SIZE][MAP_MAX_SIZE][MAP_MAX_SIZE];
	PFComponent* tar[MAP_MAX_SIZE][MAP_MAX_SIZE][MAP_MAX_SIZE];

	memset(dp, -1, sizeof(dp));
	for (auto &building : PlayerManager::getInstance()->getDefPlayer()->getBuildings()){
		for (int tr = 0; tr < building->getOccupy().X; tr++) for (int tc = 0; tc < building->getOccupy().Y; tc++){
			dp[(int)building->getCoord().x + tr][(int)building->getCoord().y + tc][building->getZ()] = INF;
			tar[(int)building->getCoord().x + tr][(int)building->getCoord().y + tc][building->getZ()] = building;
		}
	}

	queue <Vec3> Q;
	Vec3 start = Vec3(_coord.x, _coord.y, _z);
	Q.push(start);
	dp[(int)start.x][(int)start.y][(int)start.z] = 0;

	MapModel *mm = MapModel::getModel();
	while (!Q.empty()){
		Vec3 now = Q.front(); Q.pop();
		for (int k = 0; k < 4; k++)	for (int hofs = 1; hofs >= -1; hofs--){
			Vec3 ntp = now;
			ntp.x += OFFX[k];
			ntp.y += OFFY[k];
			ntp.z = now.z + hofs;

			if (!mm->isCoordInsideLayer(MapPoint(ntp.x, ntp.y), ntp.z)) continue;
			if (!mm->isCoordInsideLayer(MapPoint(ntp.x, ntp.y), ntp.z - 1)) continue;

			if (dp[(int)ntp.x][(int)ntp.y][(int)ntp.z] == INF){
				_target = tar[(int)ntp.x][(int)ntp.y][(int)ntp.z];
				Vec3 backend = now;
				while (backend != start){
					int revdir = rec[(int)backend.x][(int)backend.y][(int)backend.z].X;
					int revhofs = rec[(int)backend.x][(int)backend.y][(int)backend.z].Y;
					backend.x -= OFFX[revdir];
					backend.y -= OFFY[revdir];
					backend.z -= revhofs;
					_moves.pushBack(CMDMove::order(0, this, revdir, revhofs));
				}
				return true;
			}

			auto current = mm->getTileGIDAt(MapPoint(ntp.x, ntp.y), ntp.z);
			auto below = mm->getTileGIDAt(MapPoint(ntp.x, ntp.y), ntp.z - 1);

			if (current != mm->EMPTY_TILE) continue;
			if (below == mm->EMPTY_TILE) continue;

			Value props = mm->getTileMap()->getPropertiesForGID(below);
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

			if (mm->canMoveTo(MapPoint(ntp.x, ntp.y), ntp.z) && dp[(int)ntp.x][(int)ntp.y][(int)ntp.z] == -1){
				dp[(int)ntp.x][(int)ntp.y][(int)ntp.z] = dp[(int)now.x][(int)now.y][(int)now.z] + 1;
				rec[(int)ntp.x][(int)ntp.y][(int)ntp.z] = MP(k, hofs);
				Q.push(ntp);
				break;
			}
		}
	}
	return false;
}

void Troop::AddCMDMove(CMD* cmdMove){
	_moves.pushBack(cmdMove);
}

void Troop::go(int dir, int ohfs){
	_z += ohfs;
	switch (dir){
	case 0: goLeft(); break;
	case 1: goDown(); break;
	case 2: goRight(); break;
	case 3: goUp(); break;
	default: CCASSERT(false, "dir>=0 && dir<=3");
	}
}

void Troop::goLeft(){
	_coord.x--;
}
void Troop::goDown(){
	_coord.y--;
}
void Troop::goRight(){
	_coord.x++;
}
void Troop::goUp(){
	_coord.y++;
}