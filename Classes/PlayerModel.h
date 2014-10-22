#pragma once
#include "cocos2d.h"
#include "AppMacro.h"
#include "Building.h"
#include "Troop.h"
#include "CMD.h"
#include <map>
#include "Subject.h"
#include "MapModel.h"

using namespace std;
USING_NS_CC;

class PlayerModel :public Ref,public Subject{

public:
	PlayerModel();
	PlayerModel(string uid);
	~PlayerModel();

	void init();

	bool canAddBuilding(int BID);
	bool canAddTroop(int TID);

	void consumeResourceByBuilding(int BID);
	void consumeResourceByTroop(int TID);

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

	CC_SYNTHESIZE(int, _troopCnt, TroopCnt);
	CC_SYNTHESIZE(int, _buildingCnt, BuildingCnt);

	//CC_SYNTHESIZE(tm, _loginTime, LoginTime);

	int height;
	Point coord;
    
    //G+
    int getLstr();
    void setLstr(int value);
    int getGmag();
    void setGmag(int value);
    int getFood();
    void setFood(int value);

    float getLStrGenRate();
    void setLStrGenRate(float value);
    float getGMagGenRate();
    void setGMagGenRate(float value);
    float getFoodGenRate();
    void setFoodGenRate(float value);
    
    MapModel::Weather getPlayerOwnMapWeather();
    void setPlayerOwnMapWeather(MapModel::Weather weather);
    
    string getPlayerOwnMapCoord();
    void setPlayerOwnMapCoord(string coord);
    
    void setPlayerOwnedSwordMan(int value);
    int getPlayerOwnedSwordMan();
    void setPlayerOwnedArcher(int value);
    int getPlayerOwnedArcher();
    void setPlayerOwnedPriest(int value);
    int getPlayerOwnedPriest();
    void setPlayerOwnedMagician(int value);
    int getPlayerOwnedMagician();
    
    //NET
    void sendResourceAddNotify(int addedGPower,int addedLMana,int addedFood);
    //
protected:
	int getProperTroopOID();
	int getProperBuildingOID();
	bool canAddTroop(Troop* target);
	bool canAddBuilding(Building* target);

    int _lStr;
    int _gMag;
    int _food;
    
    double _lStrGenRate;
    double _gMagGenRate;
    double _foodGenRate;
    
    int _playerOwnedSwordMan;
    int _playerOwnedArcher;
    int _playerOwnedPriest;
    int _playerOwnedMagician;
    
    MapModel::Weather _playerOwnMapWeather;
    
    string _playerOwnMapCoord;
private:
	bitset <PLAYER_MAX_TROOPS> usedTroopOID;
	bitset <PLAYER_MAX_BUILDINGS> usedBuildingOID;
    
    //sql cb
    static int UserQueryCB(void* para,int columns,char** columnValue,char** columnName);
};