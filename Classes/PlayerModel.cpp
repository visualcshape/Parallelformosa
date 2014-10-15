#include "PlayerModel.h"
#include "ResourceModel.h"
#include "MapModel.h"
#include "command.h"

USING_NS_CC;

PlayerModel::PlayerModel(){
	_uid = rand() % 2 + 1;
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

void PlayerModel::readPlayerInfo(bool backup){
	ResourceModel *rm = ResourceModel::getModel();
	char buffer[1000];
	sprintf(buffer, "%d", _uid);
	string playerName = "player" + string(buffer);
	string filename = "PlayerInfo/" + playerName + ".info";
	if (backup)
		filename += ".backup";
	//@brief new acccount
	if (!rm->isFileExist(filename))
		writePlayerInfo();

	FILE *fp = rm->OpenFileR(filename);

	fscanf(fp, "%d %d %d", &_Lstr, &_Gmag, &_food);
	fscanf(fp, "%s", buffer);
	CCLOG("player %d info: %d %d %d => %s", _uid, _Lstr, _Gmag, _food, buffer);
	
	int year, month, day, hour, min, sec;
	sscanf(buffer, "%d-%d-%d_%d:%d:%d", &year, &month, &day, &hour, &min, &sec);

	time_t _time = time(NULL);
	tm prev = rm->makeSystemTime(year, month, day, hour, min, sec);
	tm now = *localtime(&_time);

	int difSec = (int)difftime(mktime(&now), mktime(&prev));
	gainLstr(difSec * 6);
	gainGmag(difSec * 6);
	gainFood(difSec * 6);

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
	}

	//@brief Write back
	writePlayerInfo();

	fclose(fp);
}

void PlayerModel::writePlayerInfo(bool backup){
	ResourceModel *rm = ResourceModel::getModel();
	char buffer[1000];
	sprintf(buffer, "%d", _uid);
	string playerName = "player" + string(buffer);
	string filename = "PlayerInfo/" + playerName + ".info";

	if (backup)
		filename += ".backup";

	FILE *fp = rm->OpenFileW(filename);
	CCASSERT(fp != nullptr, "read map info fail");

	//@brief save resoures
	fprintf(fp, "%d %d %d\n", _Lstr, _Gmag, _food);

	//@brief save logout time
	fprintf(fp, "%s\n", rm->getSystemTimeString().c_str());

	//@brief save troops info
	for (auto &troop : _troops){
		fprintf(fp, "troop %d %d (%.0f,%.0f,%d)\n", troop->getOID(), troop->getID() - 18, troop->getCoord().x, troop->getCoord().y, troop->getZ());
	}

	/*
	//@brief save buildings info
	for (auto &building : _buildings){
		fprintf(fp, "building %d %d (%.0f,%.0f,%d)\n", building->getOID(), building->getID() - 36, building->getCoord().x, building->getCoord().y, building->getZ());
	}*/

	fclose(fp);
}
