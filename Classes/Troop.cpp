#include "ResourceModel.h"
#include "MapModel.h"
#include "Troop.h"

bool Troop::init(){
	if (!Sprite::init())
		return false;
	return true;
}

Troop* Troop::addTroop(int TID){
	CCASSERT(TID >= 1 && TID <= 5, "TID index outside");
	ResourceModel *rm = ResourceModel::getModel();

	Troop* troop = Troop::create();
	troop->sprite = Sprite::create(rm->strCharacter[TID]);
	troop->addChild(troop->sprite, 0);
	troop->id = rm->GIDCharacter[TID];
	troop->occupy = MP(1, 1);

	troop->atk = 30;
	troop->def = 15;
	troop->hp = 100;

	return troop;
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
