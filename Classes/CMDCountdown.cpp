#include "command.h"

USING_NS_CC;

CMDCountdown::CMDCountdown(){

}

CMDCountdown::~CMDCountdown(){

}

void CMDCountdown::init(){
}

CMDCountdown* CMDCountdown::order(BattleModel* battle){
	CMDCountdown* cmdCountdown = new CMDCountdown();
	cmdCountdown->_battle = battle;
	return cmdCountdown;
}

bool CMDCountdown::execute(){
	CCLOG("========= CMDCountdown::execute()");
	_battle->timePass();
	return true;
}
