#include "ResourceModel.h"
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
	return troop;
}
