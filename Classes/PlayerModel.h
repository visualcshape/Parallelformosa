#pragma once
#include "cocos2d.h"
#include "AppMacro.h"
#include "Building.h"
#include "Troop.h"
#include "CMD.h"
#include <map>

using namespace std;
USING_NS_CC;

class PlayerModel :public Ref{

public:
	PlayerModel();
	PlayerModel(int uid);
	~PlayerModel();

	void init();
	static PlayerModel* initWithPlayer(PlayerModel* rhs);

	bool canAddTroop(int TID);
	void consumeResource(int TID);

	void changeUID(string uid);

	void readPlayerInfo(bool backup = false);
	void writePlayerInfo(bool backup = false);

	void produce(float dt);

	//@resource
	void gainLstr(int value);
	void gainGmag(int value);
	void gainFood(int value);

	void addBuilding(Building* building);
	void addTroop(Troop* troop);
	void removeBuilding(Building* building);
	void removeTroop(Troop* troop);

	void commandAttack();
	void replay();

	void addCMDStateToTroop(int timing, int oid, int adjustHp);
	void addCMDStateToBuilding(int timing, int oid, int adjustHp);
	void addCMDMoveToTroop(int timing, int oid, int dir, int hofs);

	CC_SYNTHESIZE(string, _uid, UID);
	CC_SYNTHESIZE(Vector <Building*>, _buildings, Buildings);
	CC_SYNTHESIZE(Vector <Troop*>, _troops, Troops);

	CC_SYNTHESIZE(int, _Lstr, Lstr);
	CC_SYNTHESIZE(int, _Gmag, Gmag);
	CC_SYNTHESIZE(int, _food, Food);

	CC_SYNTHESIZE(int, _troopCnt, TroopCnt);
	CC_SYNTHESIZE(int, _buildingCnt, BuildingCnt);

	//CC_SYNTHESIZE(tm, _loginTime, LoginTime);

	int height;
	Point coord;

protected:
	int getProperTroopOID();
	int getProperBuildingOID();
	bool canAddTroop(Troop* target);
	bool canAddBuilding(Building* target);

private:
	bitset <PLAYER_MAX_TROOPS> usedTroopOID;
	bitset <PLAYER_MAX_BUILDINGS> usedBuildingOID;
    
    //sql cb
    static int UserQueryCB(void* para,int columns,char** columnValue,char** columnName);
};