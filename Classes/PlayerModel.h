#pragma once
#include "cocos2d.h"
#include "Building.h"
#include "Troop.h"

USING_NS_CC;

class PlayerModel :public Ref{

public:
	PlayerModel();
	~PlayerModel();

	void init();
	bool canAddTroop(int TID);
	void consumeResource(int TID);

	void changeUID(int uid);

	//void writePlayerInfo();
	//void readPlayerInfo();

	CC_SYNTHESIZE(int, _uid, UID);
	CC_SYNTHESIZE(Vector <Building*>, _buildings, Buildings);
	CC_SYNTHESIZE(Vector <Troop*>, _troops, Troops);

	void addBuilding(Building* building);
	void addTroop(Troop* troop);
	void removeBuilding(Building* building);
	void removeTroop(Troop* troop);
	void commandAttack();

	int height;
	Point coord;

	int L_str;
	int G_mag;
	int food;

private:
	void loadData();
	void saveData();
};