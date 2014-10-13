#include "PlayerModel.h"
#include "command.h"
#include "CommandModel.h"

USING_NS_CC;

PlayerModel::PlayerModel(){
	_uid = rand() % 2 + 1;
	init();
}

PlayerModel::~PlayerModel(){
	writePlayerInfo();
}

bool PlayerModel::init(){
	if (!Layer::init())
		return false;
	height = -1;
	_buildings.clear();
	_troops.clear();
	_Lstr = _Gmag = 3000;
	_food = 30;
	readPlayerInfo();

	return true;
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

	_Lstr -= rm->costLstr[TID];
	_Gmag -= rm->costGmag[TID];;
	_food -= rm->costFood[TID];;
}

void PlayerModel::changeUID(int uid){
	writePlayerInfo();
	_uid = uid;
	readPlayerInfo();
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
	_buildings.pushBack(building);
}

void PlayerModel::addTroop(Troop* troop){
	_troops.pushBack(troop);
}

void PlayerModel::removeBuilding(Building* building){
	_buildings.eraseObject(building);
}

void PlayerModel::removeTroop(Troop* troop){
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

void PlayerModel::readPlayerInfo(){
	ResourceModel *rm = ResourceModel::getModel();
	char buffer[1000];
	sprintf(buffer, "%d", _uid);
	string playerName = "player" + string(buffer);
	string filename = "PlayerInfo/" + playerName + ".info";

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
	writePlayerInfo();

	fclose(fp);
}

void PlayerModel::writePlayerInfo(){
	ResourceModel *rm = ResourceModel::getModel();
	char buffer[1000];
	sprintf(buffer, "%d", _uid);
	string playerName = "player" + string(buffer);
	string filename = "PlayerInfo/" + playerName + ".info";

	FILE *fp = rm->OpenFileW(filename);
	CCASSERT(fp != nullptr, "read map info fail");

	fprintf(fp, "%d %d %d\n", _Lstr, _Gmag, _food);
	fprintf(fp, "%s\n", rm->getSystemTimeString().c_str());
	fclose(fp);
}
