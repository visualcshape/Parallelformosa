#include "BattleModel.h"
#include "jansson.h"
#include <pomelo.h>
#include "CCPomeloWrapper.h"
#include "MapModel.h"
#include "json.h"
#include "SceneManager.h"
#include "PlayerManager.h"
#include "PlayerModel.h"
#include "Command.h"

USING_NS_CC;

BattleModel* BattleModel::bm_pInstance;

BattleModel::BattleModel(){
}

BattleModel::~BattleModel(){
}

BattleModel* BattleModel::getModel(){
	if (bm_pInstance == nullptr)
		bm_pInstance = new BattleModel();
	return bm_pInstance;
}

void BattleModel::setupBattle(MapModel* mapModel, bool isSimulation){
	_mapModel = mapModel;
	_isSimulation = isSimulation;
}

void BattleModel::sendRequest(Vector <Building*> buildings){
	Json::Value msg;
	Json::FastWriter writer;
	for (auto &building : buildings){
		Json::Value info;
		info["id"] = building->getID();
		info["coord"]["X"] = (int)building->getCoord().x;
		info["coord"]["Y"] = (int)building->getCoord().y;
		info["coord"]["Z"] = building->getZ();

		info["owner"] = (int)building->getOwner();
		info["occupy"]["X"] = (int)building->getOccupy().X;
		info["occupy"]["Y"] = (int)building->getOccupy().Y;
		info["atk"] = building->getAtk();
		info["def"] = building->getDef();
		info["hp"] = building->getHp();
		
		info["range"] = building->getRange();

		msg.append(info);
	}
}

void BattleModel::sendRequest(Vector <Troop*> troops){
	Json::Value msg;
	for (auto &troop : troops){
		Json::Value info;
		info["id"] = troop->getID();
		info["coord"]["X"] = (int)troop->getCoord().x;
		info["coord"]["Y"] = (int)troop->getCoord().y;
		info["coord"]["Z"] = troop->getZ();

		info["owner"] = (int)troop->getOwner();
		info["occupy"]["X"] = (int)troop->getOccupy().X;
		info["occupy"]["Y"] = (int)troop->getOccupy().Y;
		info["atk"] = troop->getAtk();
		info["def"] = troop->getDef();
		info["hp"] = troop->getHp();

		msg.append(info);
	}
}

void BattleModel::timePass(){
	_countdown--;
	char buffer[70];
	sprintf(buffer, "time remain: %d", _countdown);
	MapModel::getModel()->getlblCountdownPos()->setString(buffer);

	PlayerManager::getInstance()->getAtkPlayer()->commandAttack();
	PlayerManager::getInstance()->getDefPlayer()->commandAttack();

	if (_countdown == 0)
		battleOver(PlayerManager::getInstance()->getDefPlayer());
	else if (PlayerManager::getInstance()->getAtkPlayer()->getTroops().empty())
		battleOver(PlayerManager::getInstance()->getDefPlayer());
	else if (PlayerManager::getInstance()->getDefPlayer()->getBuildings().empty())
		battleOver(PlayerManager::getInstance()->getAtkPlayer());
}

void BattleModel::battleOver(PlayerModel* winPlayer){
	CCLOG("BattleModel::battleOver");
	if (CMDFileStream::getInstance()->isStreamOn())
		CMDFileStream::getInstance()->execute();

	if (_isSimulation){
		_isSimulation = false;

		PlayerManager::getInstance()->getAtkPlayer()->init();
		PlayerManager::getInstance()->getDefPlayer()->init();

		PlayerManager::getInstance()->getAtkPlayer()->readPlayerInfo(true);
		PlayerManager::getInstance()->getDefPlayer()->readPlayerInfo(true);
		_mapModel->readMapInfo(true);
	}
	else{
		if (winPlayer->getUID() == PlayerManager::getInstance()->getAtkPlayer()->getUID()){
			SceneManager::goBattleOverScreen("ATTACK WIN!!");
		}
		else if (winPlayer->getUID() == PlayerManager::getInstance()->getDefPlayer()->getUID()){
			PlayerManager::getInstance()->getAtkPlayer()->height = -1;
			SceneManager::goBattleOverScreen("DEFENSE WIN!!");
		}
		else{
			CCASSERT(false, "so, ... who win?!");
		}
	}
}

void BattleModel::startBattle(){
	_countdown = 60;
	if (_isSimulation){
		PlayerManager::getInstance()->getAtkPlayer()->writePlayerInfo(true);
		PlayerManager::getInstance()->getDefPlayer()->writePlayerInfo(true);
		_mapModel->writeMapInfo(true);

		CMDFileStream::getInstance()->OpenStream(_mapModel->getMapName() + ".cmd");
	}
}

void BattleModel::readCommandConfig(){
	ResourceModel *rm = ResourceModel::getModel();
	string filename = "CommandInfo/" + _mapModel->getMapName() + ".cmd";
	FILE* fp = rm->OpenFileR(filename);

	CCASSERT(fp != nullptr, "readCommandConfig fail");

	char buffer[1000];
	while (~fscanf(fp, "%s", buffer)){
		if (strcmp(buffer, "CMDState") == 0){

		}
		else if (strcmp(buffer, "CMDMove") == 0){

		}
	}
	sprintf(buffer, "CMDState troop %d %d %d\n", pcp->getOwner(), pcp->getOID(), _adjustHp);
	sprintf(buffer, "CMDMove troop %d %d %d %d\n", _troop->getOwner(), _troop->getOID(), _dir, _hofs);
}
