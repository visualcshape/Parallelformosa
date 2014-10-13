#include "ResourceModel.h"
#include "Building.h"
#include "MapModel.h"

Building::Building(){
}

Building::~Building(){
}

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
	const int ATK[6] = { -1, 30, 40, 50, 60, 70 };
	const int DEF[6] = { -1, 20, 15, 10, 5, 0 };
	const int HP[6] = { -1, 100, 150, 200, 150, 100 };

	CCASSERT(BID >= 1 && BID <= 5, "BID not found");
	building->occupy = OCCUPY[BID];
	building->atk = ATK[BID];
	building->def = DEF[BID];
	building->hp = HP[BID];

	return building;
}

void Building::attackLogic(){
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
	}
}