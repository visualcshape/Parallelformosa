#include "ResourceModel.h"
#include "PFComponent.h"
#include "MapModel.h"
#include "Command.h"

PFComponent::PFComponent(){
}

PFComponent::~PFComponent(){
}

bool PFComponent::init(){
	if (!Sprite::init())
		return false;
	_oid = -1;
	return true;
}

void PFComponent::AddCMDState(CMD* cmdState){
	CCLOG("PFComponent::AddCMDState()");
	_states.pushBack(cmdState);
}
void PFComponent::AddHp(int value){
	_hp += value;
}

bool PFComponent::isDead(){
	return _hp <= 0;
}

bool PFComponent::canAtttack(PFComponent* target){
	for (int tr = 0; tr < target->_occupy.X; tr++) for (int tc = 0; tc < target->_occupy.Y; tc++){
		Point pt = Point(target->getCoord().x + tr, target->getCoord().y + tc);
		Point dist = pt - getCoord();
		if (MAX(abs(dist.x), abs(dist.y)) <= 1)
			return true;
	}
	return false;
}

bool PFComponent::tryToAttack(){
	PlayerModel* targetPlayer = nullptr;
	if (_owner == MapModel::getModel()->getAtkPlayer()->getUID())
		targetPlayer = MapModel::getModel()->getDefPlayer();
	else
		targetPlayer = MapModel::getModel()->getAtkPlayer();

	CC_ASSERT(targetPlayer != nullptr);

	bool beenAttack = false;

	//@brief attack defendant buildings
	for (auto &target : targetPlayer->getBuildings()){
		if (canAtttack(target)){
			target->AddCMDState(CMDState::order(target, -MAX(0, _atk - target->getDef())));
			beenAttack = true;
		}
	}

	//@brief attack defendant troops
	for (auto &target : targetPlayer->getTroops()){
		if (canAtttack(target)){
			target->AddCMDState(CMDState::order(target, -MAX(0, _atk - target->getDef())));
			beenAttack = true;
		}
	}

	return beenAttack;
}

