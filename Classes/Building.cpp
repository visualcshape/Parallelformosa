#include "ResourceModel.h"
#include "Building.h"

bool Building::init(){
	if (!Sprite::init())
		return false;
	return true;
}

Building* Building::build(int BID){
	ResourceModel *rm = ResourceModel::getModel();

	Building* building = Building::create();
	building->sprite = Sprite::create(rm->strBuilding[BID]);
	building->addChild(building->sprite, 0);
	building->id = rm->GIDBuilding[BID];
	building->range = 200;

	switch (BID){
	case 1: building->occupy = MP(1, 1); break;
	case 2: building->occupy = MP(2, 2); break;
	case 3: building->occupy = MP(3, 3); break;
	case 4: building->occupy = MP(2, 1); break;
	case 5: building->occupy = MP(1, 3); break;
	default: CCASSERT(false, "BID not found"); break;
	}

	return building;
}
