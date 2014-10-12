#include "PlayerModel.h"

USING_NS_CC;


PlayerModel::PlayerModel(){
	_uid = rand() % 2 + 1;
	L_str = G_mag = 3000;
	food = 30;
	init();
}

PlayerModel::~PlayerModel(){
}

void PlayerModel::init(){
	height = -1;
	_buildings.clear();
	_troops.clear();
}

bool PlayerModel::canAddTroop(int TID){
	ResourceModel *rm = ResourceModel::getModel();
	return L_str >= rm->costLstr[TID] &&
		G_mag >= rm->costGmag[TID] &&
		food >= rm->costFood[TID];
}

void PlayerModel::consumeResource(int TID){
	ResourceModel *rm = ResourceModel::getModel();

	L_str -= rm->costLstr[TID];
	G_mag -= rm->costGmag[TID];;
	food -= rm->costFood[TID];;
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
/*
void PlayerModel::writePlayerInfo(){
	ResourceModel *rm = ResourceModel::getModel();

	char buffer[1000];
	sprintf(buffer, "%d", _uid);
	string prefix = "player" + string(buffer);
	std::string filename = "MapInfo/" + prefix + "/" + rm->strWorldMap + ".info";

	FILE *fp = rm->OpenFileW(filename);
	CCASSERT(fp != nullptr, "write player info fail");

	for (auto &building : _buildings)
		fprintf(fp, "%d %.0f %.0f %d %d\n", building->id, building->getCoord().x, building->getCoord().y, building->height);
	fclose(fp);
}

void PlayerModel::readPlayerInfo(){
	ResourceModel *rm = ResourceModel::getModel();

	char buffer[1000];
	sprintf(buffer, "%d", _uid);
	string prefix = "player" + string(buffer);
	std::string filename = "MapInfo/" + prefix + "/" + rm->strWorldMap + ".info";

	FILE *fp = rm->OpenFileW(filename);
	CCASSERT(fp != nullptr, "write player info fail");

	int id, x, y, height;
	while (~fscanf(fp, "%d %d %d %d %d", &id, &x, &y, &height)){
		addBuildingToMap(id, MapPoint(x, y), height);
		CCLOG("info: %d %d %d %d %d", id, x, y, height, owner);
	}
	fclose(fp);

	CCLOG(">> _baseBuidling = nullptr => %s", _curPlayer->height == -1 ? "Yes" : "No");
	if (_curPlayer->height != -1 && rm->strWorldMap == mapName){
		CCLOG(">> after win, delete basebuilding");
		//pm->_baseBuilding->sett
		Building* tmp = Building::build(1);
		tmp->height = _curPlayer->height;
		tmp->setCoord(_curPlayer->coord);
		buildingDelete(tmp);
		_curPlayer->height = -1;
	}
	CCLOG(">> _baseBuidling = nullptr => %s", _curPlayer->height == -1 ? "Yes" : "No");
}
*/