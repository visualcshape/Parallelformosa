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
	_isReplay = false;
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

		info["owner"] = building->getOwner();
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

		info["owner"] = troop->getOwner();
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

	if (_isReplay){
		CCLOG("replay");
		PlayerManager::getInstance()->getAtkPlayer()->replay();
		PlayerManager::getInstance()->getDefPlayer()->replay();
	}
	else{
		PlayerManager::getInstance()->getAtkPlayer()->commandAttack();
		PlayerManager::getInstance()->getDefPlayer()->commandAttack();
	}

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

	_isReplay = false;
	if (_isSimulation){
		_isSimulation = false;

		PlayerManager::getInstance()->getAtkPlayer()->init();
		PlayerManager::getInstance()->getDefPlayer()->init();

		PlayerManager::getInstance()->getAtkPlayer()->readPlayerInfo(true);
		PlayerManager::getInstance()->getDefPlayer()->readPlayerInfo(true);
		_mapModel->readMapInfo(true);

		readCommandConfig();
		_isReplay = true;
		startBattle();
	}
	if(!_isReplay){
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
	string filename = _mapModel->getMapName() + ".cmd";
	FILE* fp = rm->OpenFileR(filename);

	CCASSERT(fp != nullptr, "readCommandConfig fail");

	auto atkPlayer = PlayerManager::getInstance()->getAtkPlayer();
	auto defPlayer = PlayerManager::getInstance()->getDefPlayer();
	int timing;
	while (~fscanf(fp, "%d", &timing)){
		char buffer[1000];
		fscanf(fp, "%s", buffer);
		if (strcmp(buffer, "CMDState") == 0){
            int oid, adjustHp;
            std::string owner;
            char w[256];
            strcpy(w, owner.c_str());
			fscanf(fp, "%s %s %d %d", buffer, w, &oid, &adjustHp);
			if (strcmp(buffer, "troop") == 0){
				if (owner == atkPlayer->getUID())
					atkPlayer->addCMDStateToTroop(timing, oid, adjustHp);
				else if (owner == defPlayer->getUID())
					defPlayer->addCMDStateToTroop(timing, oid, adjustHp);
			}
			else if (strcmp(buffer, "building") == 0){
				if (owner == atkPlayer->getUID())
					atkPlayer->addCMDStateToBuilding(timing, oid, adjustHp);
				else if (owner == defPlayer->getUID())
					defPlayer->addCMDStateToBuilding(timing, oid, adjustHp);
			}
		}
		else if (strcmp(buffer, "CMDMove") == 0){
            /*///wait to repair...
			int owner, oid, dir, hofs;
            
			fscanf(fp, "%*s %d %d %d %d", &owner, &oid, &dir, &hofs);
			if (owner == atkPlayer->getUID())
				atkPlayer->addCMDMoveToTroop(timing, oid, dir, hofs);
			else if (owner == defPlayer->getUID())
				defPlayer->addCMDMoveToTroop(timing, oid, dir, hofs);
            //....*/
		}
	}
}
