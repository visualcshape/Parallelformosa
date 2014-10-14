#include "BattleModel.h"
#include "jansson.h"
#include <pomelo.h>
#include "CCPomeloWrapper.h"
#include "MapModel.h"
#include "json.h"
#include "SceneManager.h"

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

void BattleModel::sendRequest(Vector <Building*> buildings){
	Json::Value msg;
	Json::FastWriter writer;
	for (auto &building : buildings){
		Json::Value info;
		info["id"] = building->id;
		info["coord"]["X"] = (int)building->getCoord().x;
		info["coord"]["Y"] = (int)building->getCoord().y;
		info["coord"]["Z"] = building->height;

		info["owner"] = (int)building->owner;
		info["occupy"]["X"] = (int)building->occupy.X;
		info["occupy"]["Y"] = (int)building->occupy.Y;
		info["atk"] = building->atk;
		info["def"] = building->def;
		info["hp"] = building->hp;
		
		info["range"] = building->range;

		msg.append(info);
	}
}

void BattleModel::sendRequest(Vector <Troop*> troops){
	Json::Value msg;
	for (auto &troop : troops){
		Json::Value info;
		info["id"] = troop->id;
		info["coord"]["X"] = (int)troop->getCoord().x;
		info["coord"]["Y"] = (int)troop->getCoord().y;
		info["coord"]["Z"] = troop->height;

		info["owner"] = (int)troop->owner;
		info["occupy"]["X"] = (int)troop->occupy.X;
		info["occupy"]["Y"] = (int)troop->occupy.Y;
		info["atk"] = troop->atk;
		info["def"] = troop->def;
		info["hp"] = troop->hp;

		msg.append(info);
	}
}

void BattleModel::timePass(){
	_countdown--;
	char buffer[70];
	sprintf(buffer, "time remain: %d", _countdown);
	MapModel::getModel()->getlblCountdownPos()->setString(buffer);
	if (_countdown == 0)
		battleOver();
}

void BattleModel::battleOver(){
	MapModel::getModel()->getAtkPlayer()->height = -1;
	SceneManager::goBattleOverScreen("DEFENSE WIN!!");
}
