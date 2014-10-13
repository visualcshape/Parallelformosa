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

	CC_SYNTHESIZE(int, _Lstr, Lstr);
	CC_SYNTHESIZE(int, _Gmag, Gmag);
	CC_SYNTHESIZE(int, _food, Food);

	//@resource
	void gainLstr(int value);
	void gainGmag(int value);
	void gainFood(int value);

	void addBuilding(Building* building);
	void addTroop(Troop* troop);
	void removeBuilding(Building* building);
	void removeTroop(Troop* troop);
	void commandAttack();

	int height;
	Point coord;


private:
	void readPlayerInfo();
	void writePlayerInfo();
};