#include "BattleModel.h"
#include "jansson.h"
#include <pomelo.h>
#include "CCPomeloWrapper.h"
#include "MapModel.h"
#include "json.h"
#include "SceneManager.h"
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

void BattleModel::setupBattle(PlayerModel* atkPlayer, PlayerModel* defPlayer, MapModel* mapModel){
	_atkPlayer = atkPlayer;
	_defPlayer = defPlayer;
	_mapModel = mapModel;
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

	_atkPlayer->commandAttack();
	_defPlayer->commandAttack();
	CCLOG("attack mode activate!!!!");

	if (_countdown == 0)
		battleOver(_defPlayer);
	else if (_atkPlayer->getTroops().empty())
		battleOver(_defPlayer);
	else if (_defPlayer->getBuildings().empty())
		battleOver(_atkPlayer);
}

void BattleModel::battleOver(PlayerModel* winPlayer){
	if (CMDFileStream::getInstance()->isStreamOn())
		CMDFileStream::getInstance()->execute();

	if (winPlayer->getUID() == _atkPlayer->getUID()){
		SceneManager::goBattleOverScreen("ATTACK WIN!!");
	}
	else if (winPlayer->getUID() == _defPlayer->getUID()){
		_atkPlayer->height = -1;
		SceneManager::goBattleOverScreen("DEFENSE WIN!!");
	}
	else{
		CCASSERT(false, "so, ... who win?!");
	}
}

void BattleModel::simulateBattle(){
	//PlayerModel* copyOfAtkPlayer = PlayerModel::initWithPlayer(_atkPlayer);
	//PlayerModel* copyOfDefPlayer = PlayerModel::initWithPlayer(_defPlayer);
	_atkPlayer->writePlayerInfo(true);
	_defPlayer->writePlayerInfo(true);
	_mapModel->writeMapInfo(true);

	CMDFileStream::getInstance()->OpenStream(_mapModel->getMapName() + ".cmd");
	

}

void BattleModel::doBattle(){
	_mapModel->commandAttack();
}
