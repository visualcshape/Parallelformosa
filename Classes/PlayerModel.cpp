#include "PlayerModel.h"
#include "ResourceModel.h"
#include "MapModel.h"
#include "command.h"

USING_NS_CC;

PlayerModel::PlayerModel(){
	_uid = rand() % 2 + 1;
	init();
}

PlayerModel::PlayerModel(int uid){
	_uid = uid;
	init();
}

PlayerModel::~PlayerModel(){
	writePlayerInfo();
}

void PlayerModel::init(){
	height = -1;
	_buildings.clear();
	usedTroopOID.reset();
	usedBuildingOID.reset();
	_troops.clear();
	_Lstr = _Gmag = 3000;
	_food = 30;
	readPlayerInfo();
}

PlayerModel* PlayerModel::initWithPlayer(PlayerModel* rhs){
	PlayerModel* lhs = new PlayerModel();
	lhs->coord = rhs->coord;
	lhs->height = rhs->height;
	lhs->setBuildings(rhs->getBuildings());
	lhs->setFood(rhs->getFood());
	lhs->setGmag(rhs->getGmag());
	lhs->setLstr(rhs->getLstr());
	lhs->setTroops(rhs->getTroops());
	lhs->setUID(rhs->getUID());
	return lhs;
}

void PlayerModel::produce(float dt){
	CMDResource::order(this, 6, 6, 6)->execute();
	//CommandModel::getModel()->AddCommand(CMDResource::order(this, 6, 6, 6));
}

bool PlayerModel::canAddTroop(int TID){
	ResourceModel *rm = ResourceModel::getModel();
	return _Lstr >= rm->costLstr[TID] &&
		_Gmag >= rm->costGmag[TID] &&
		_food >= rm->costFood[TID];
}

void PlayerModel::consumeResource(int TID){
	ResourceModel *rm = ResourceModel::getModel();
	CMDResource::order(this, -rm->costLstr[TID], -rm->costGmag[TID], -rm->costFood[TID])->execute();
}

void PlayerModel::changeUID(int uid){
	writePlayerInfo();
	_uid = uid;
	init();
}

void PlayerModel::gainLstr(int value){
	_Lstr += value;
	_Lstr = max(_Lstr, 0);
}

void PlayerModel::gainGmag(int value){
	_Gmag += value;
	_Gmag = max(_Gmag, 0);
}

void PlayerModel::gainFood(int value){
	_food += value;
	_food = max(_food, 0);
}

void PlayerModel::addBuilding(Building* building){
	if (!canAddBuilding(building)) return;
	if (building->getOID() == -1)
		building->setOID(getProperBuildingOID());
	CC_ASSERT(!usedBuildingOID[building->getOID()]);
	usedBuildingOID[building->getOID()] = true;
	_buildings.pushBack(building);
}

void PlayerModel::addTroop(Troop* troop){
	if (!canAddTroop(troop)) return;
	if (troop->getOID() == -1)
		troop->setOID(getProperTroopOID());
	CC_ASSERT(!usedTroopOID[troop->getOID()]);
	usedTroopOID[troop->getOID()] = true;
	_troops.pushBack(troop);
}

void PlayerModel::removeBuilding(Building* building){
	CC_ASSERT(usedBuildingOID[building->getOID()]);
	usedBuildingOID[building->getOID()] = false;
	_buildings.eraseObject(building);
}

void PlayerModel::removeTroop(Troop* troop){
	CC_ASSERT(usedTroopOID[troop->getOID()]);
	usedTroopOID[troop->getOID()] = false;
	_troops.eraseObject(troop);
}

void PlayerModel::commandAttack(){
	auto _tmp_troops = _troops;
	auto _tmp_buildings = _buildings;
	for (auto &troop : _tmp_troops)
		troop->attackLogic();
	for (auto &building : _tmp_buildings)
		building->attackLogic();
}

void PlayerModel::replay(){
	auto _tmp_troops = _troops;
	auto _tmp_buildings = _buildings;
	for (auto &troop : _tmp_troops)
		troop->replayLogic();
	for (auto &building : _tmp_buildings)
		building->replayLogic();

}

int PlayerModel::getProperTroopOID(){
	for (int oid = 0; oid < PLAYER_MAX_TROOPS; oid++) if (!usedTroopOID[oid]){
		return oid;
	}
	CCASSERT(false, "PLAYER_MAX_TROOPS is not enough");
	return -1;
}

int PlayerModel::getProperBuildingOID(){
	for (int oid = 0; oid < PLAYER_MAX_BUILDINGS; oid++) if (!usedBuildingOID[oid]){
		return oid;
	}
	CCASSERT(false, "PLAYER_MAX_BUILDINGS is not enough");
	return -1;
}

bool PlayerModel::canAddTroop(Troop* target){
	for (auto &troop : _troops){
		if (troop->getCoord() == target->getCoord() && troop->getZ() == target->getZ())
			return false;
	}
	return true;
}

bool PlayerModel::canAddBuilding(Building* target){
	for (auto &building : _buildings){
		if (building->getCoord() == target->getCoord() && building->getZ() == target->getZ())
			return false;
	}
	return true;

}

void PlayerModel::addCMDStateToTroop(int timing, int oid, int adjustHp){
	CCLOG("addCMDStateToTroop");
	for (auto &troop : _troops) if (troop->getOID() == oid){
		troop->AddCMDState(CMDState::order(timing, troop, adjustHp));
		return;
	}
    CC_ASSERT(false);
    //problem
	//CCASSERT(false, "didn't find proper troop with oid = %d", oid);
}
void PlayerModel::addCMDStateToBuilding(int timing, int oid, int adjustHp){
	CCLOG("addCMDStateToBuilding");
	for (auto &building : _buildings) if (building->getOID() == oid){
		building->AddCMDState(CMDState::order(timing, building, adjustHp));
		return;
	}
    CC_ASSERT(false);
    //problem
	//CCASSERT(false, "didn't find proper building with oid = %d", oid);
}
void PlayerModel::addCMDMoveToTroop(int timing, int oid, int dir, int hofs){
	CCLOG("addCMDMoveToTroop");
	for (auto &troop : _troops) if (troop->getOID() == oid){
		troop->AddCMDMove(CMDMove::order(timing, troop, dir, hofs));
		return;
	}
    CC_ASSERT(false);
    //problem
	//CCASSERT(false, "didn't find proper troop with oid = %d", oid);
}

void PlayerModel::readPlayerInfo(bool backup){
    sqlite3* pDB = Database::getInstance()->getDatabasePointer();
    char* pzErrMsg;
    string sql = "SELECT * FROM User ORDER BY ROWID ASC LIMIT 1";
    map<string,string> pass;
    
    int result = sqlite3_exec(pDB, sql.c_str(), PlayerModel::UserQueryCB, &pass, &pzErrMsg);
    CCASSERT(result==SQLITE_OK, pzErrMsg);
    
    _uid = pass["UID"];
    _Gmag = atoi(pass["GPower"].c_str());
    _Lstr = atoi(pass["LMana"].c_str());
    _food = atoi(pass["Food"].c_str());
    
    //Deprecated...
	//ResourceModel *rm = ResourceModel::getModel();
	//char buffer[1000];
	//sprintf(buffer, "%d", _uid);
	//string playerName = "player" + string(buffer);
	//string filename = "PlayerInfo/" + playerName + ".info";
	//if (backup)
	//	filename += ".backup";
	//@brief new acccount
	//if (!rm->isFileExist(filename))
	//	writePlayerInfo(backup);

	//FILE *fp = rm->OpenFileR(filename);

	//fscanf(fp, "%d %d %d", &_Lstr, &_Gmag, &_food);
	//fscanf(fp, "%s", buffer);
	//CCLOG("player %d info: %d %d %d => %s", _uid, _Lstr, _Gmag, _food, buffer);
	
	//int year, month, day, hour, min, sec;
	//sscanf(buffer, "%d-%d-%d_%d:%d:%d", &year, &month, &day, &hour, &min, &sec);

	//time_t _time = time(NULL);
	//tm prev = rm->makeSystemTime(year, month, day, hour, min, sec);
	//tm now = *localtime(&_time);

	//int difSec = (int)difftime(mktime(&now), mktime(&prev));
	//gainLstr(difSec * 6);
	//gainGmag(difSec * 6);
	//gainFood(difSec * 6);
	/*
	while (~fscanf(fp, "%s", buffer)){
		if (strcmp(buffer, "troop") == 0){
			int oid, tid, x, y, z;
			fscanf(fp, "%d %d (%d,%d,%d)", &oid, &tid, &x, &y, &z);
			Troop* newTroop = Troop::addTroop(tid);
			newTroop->setOID(oid);
			newTroop->setCoord(Point(x, y));
			newTroop->setZ(z);
			addTroop(newTroop);
		}
		else {
			CCASSERT(false, "player info type not sure?!");
		}
	}*/

	//@brief Write back
	//writePlayerInfo(backup);

	//fclose(fp);
}

int PlayerModel::UserQueryCB(void *para, int columns, char **columnValue, char **columnName)
{
    map<string,string>* pass = static_cast<map<string,string>*>(para);
    
    for(int i = 0 ; i < columns ; i++)
    {
        string columnNameStr = columnName[i];
        string columnValueStr = columnValue[i];
        
        if(columnNameStr=="GPower")
        {
            (*pass)["GPower"] = columnValueStr;
        }
        else if(columnNameStr == "LMana")
        {
            (*pass)["LMana"] = columnValueStr;
        }
        else if (columnNameStr=="Food")
        {
            (*pass)["Food"]=columnValueStr;
        }
        else if(columnNameStr=="ID")
        {
            (*pass)["UID"] = columnValueStr;
        }
        else
        {
            continue;
        }
    }
    
    return SQLITE_OK;
}

void PlayerModel::writePlayerInfo(bool backup){
    sqlite3* pDB = Database::getInstance()->getDatabasePointer();
    char sql[128] = "";
    char* errMsg;
    
    sprintf(sql, "Update User SET GPower=%d,LMana=%d,Food=%d WHERE ID='%s'",_Gmag,_Lstr,_food,_uid.c_str());
    
    int result = sqlite3_exec(pDB, sql, nullptr, nullptr, &errMsg);
    
    CC_ASSERT(result==SQLITE_OK);
    
	//ResourceModel *rm = ResourceModel::getModel();
	//char buffer[1000];
	//sprintf(buffer, "%d", _uid);
	//string playerName = "player" + string(buffer);
	//string filename = "PlayerInfo/" + playerName + ".info";

	//if (backup)
	//	filename += ".backup";

	//FILE *fp = rm->OpenFileW(filename);
	//CCASSERT(fp != nullptr, "read map info fail");

	//@brief save resoures
	//fprintf(fp, "%d %d %d\n", _Lstr, _Gmag, _food);

	//@brief save logout time
	//fprintf(fp, "%s\n", rm->getSystemTimeString().c_str());

	/*
	//@brief save troops info
	for (auto &troop : _troops){
		fprintf(fp, "troop %d %d (%.0f,%.0f,%d)\n", troop->getOID(), troop->getID() - 18, troop->getCoord().x, troop->getCoord().y, troop->getZ());
	}*/

	/*
	//@brief save buildings info
	for (auto &building : _buildings){
		fprintf(fp, "building %d %d (%.0f,%.0f,%d)\n", building->getOID(), building->getID() - 36, building->getCoord().x, building->getCoord().y, building->getZ());
	}*/

	//fclose(fp);
}
