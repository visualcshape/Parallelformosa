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

	const PII OCCUPY[6] = { MP(0, 0), MP(1, 1), MP(2, 2), MP(3, 3), MP(2, 1), MP(1, 3) };
	const int ATK[6] = { -1, 40, 50, 60, 70, 80 };
	const int DEF[6] = { -1, 20, 15, 10, 5, 0 };
	const int HP[6] = { -1, 100, 150, 200, 150, 100 };

	CCASSERT(BID >= 1 && BID <= 5, "BID not found");
	building->occupy = OCCUPY[BID];
	building->atk = ATK[BID];
	building->def = DEF[BID];
	building->hp = HP[BID];

	return building;
}
