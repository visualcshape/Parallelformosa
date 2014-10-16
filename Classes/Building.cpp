#include "ResourceModel.h"
#include "Building.h"
#include "MapModel.h"
#include "BuildingModel.h"

Building::Building(){
}

Building::~Building(){
	_states.clear();
}

bool Building::init(){
	if (!PFComponent::init())
		return false;
	_oid = -1;
	return true;
}

Building* Building::build(int BID){
	CCASSERT(BID >= 1 && BID <= 5, "BID not found");
	ResourceModel *rm = ResourceModel::getModel();

	Building* building = Building::create();
	building->_sprite = Sprite::create(rm->strBuilding[BID]);
	building->addChild(building->_sprite, 0);
	building->_id = rm->GIDBuilding[BID];
	building->_range = 200;

	const PII OCCUPY[6] = { MP(0, 0), MP(1, 1), MP(2, 2), MP(3, 3), MP(2, 1), MP(1, 3) };
	const int ATK[6] = { -1, 30, 40, 50, 60, 70 };
	const int DEF[6] = { -1, 20, 15, 10, 5, 0 };
	const int HP[6] = { -1, 100, 150, 200, 150, 100 };

	/*auto itr = BuildingModel::getInstance()->getBuildingModelMap()->find(key);

	_gPowerValue->setString(std::to_string(itr->second.gPower));
	_lManaValue->setString(std::to_string(itr->second.lMana));
	_foodValue->setString(std::to_string(itr->second.foodCost));
	_healthValue->setString(std::to_string(itr->second.baseHP));
	_typeValue->setString(itr->second.type);
	_descriptionValue->setString(itr->second.descrption);*/

	building->_occupy = OCCUPY[BID];
	building->_atk = ATK[BID];
	building->_def = DEF[BID];
	building->_hp = HP[BID];

	return building;
}

void Building::attackLogic(){
	MapModel *mm = MapModel::getModel();
	if (isDead()){
		mm->buildingDelete(this);
		return;
	}

	if (!_states.empty()){
		_states.back()->execute();
		_states.popBack();
	}

	tryToAttack();
	/*
	MapModel *mm = MapModel::getModel();
	auto target = mm->getClosestTroop(this);

	if (target != nullptr){
		bool could_attack = false;

		for (int tr = 0; tr < occupy.X; tr++) for (int tc = 0; tc < occupy.Y; tc++){
			Point pt = Point(getCoord().x + tr, getCoord().y + tc);
			Point moveVector = pt - target->getCoord();
			if (MAX(abs(moveVector.x), abs(moveVector.y)) <= 1) could_attack = true;
		}

		if (could_attack){
			//attack troop
			CCLOG("building attack");
			target->hp -= MAX(0, atk - target->def);
			CCLOG("attack : hp = %d, atk = %d, def = %d\n", hp, atk, def);
			CCLOG("defend : hp = %d, atk = %d, def = %d\n", target->hp, target->atk, target->def);

			if (target->hp <= 0) mm->troopDelete(target);

			//troop->runAction(Sequence::create(RotateTo::create(rotateDuration, cocosAngle), NULL));
		}
	}*/
}

void Building::replayLogic(){
	MapModel *mm = MapModel::getModel();
	if (isDead()){
		mm->buildingDelete(this);
		return;
	}

	if (!_states.empty() && _states.front()->execute())
		_states.erase(_states.begin());
}
