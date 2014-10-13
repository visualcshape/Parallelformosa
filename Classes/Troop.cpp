#include "ResourceModel.h"
#include "MapModel.h"
#include "Troop.h"

Troop::Troop(){
}

Troop::~Troop(){
}

bool Troop::init(){
	if (!Sprite::init())
		return false;

	return true;
}

Troop* Troop::addTroop(int TID){
	CCASSERT(TID >= 1 && TID <= 5, "TID index outside");
	ResourceModel *rm = ResourceModel::getModel();

	Troop* troop = Troop::create();
	troop->sprite = Sprite::create(rm->strTroop[TID]);
	troop->addChild(troop->sprite, 0);
	troop->id = rm->GIDTroop[TID];
	troop->occupy = MP(1, 1);

	troop->atk = 30;
	troop->def = 15;
	troop->hp = 100;

	return troop;
}

void Troop::attackLogic(){
	MapModel *mm = MapModel::getModel();
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
				mm->troopMove(this, goDir, gohofs);
				//}
			}
			else{/*
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
				}*/
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
	}
}

void Troop::go(int dir, int ohfs){
	height += ohfs;
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