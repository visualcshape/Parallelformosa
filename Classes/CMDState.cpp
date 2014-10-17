#include "command.h"

USING_NS_CC;

CMDState::CMDState(){

}

CMDState::~CMDState(){

}

void CMDState::init(){
}

CMDState* CMDState::order(int timing, PFComponent* pfComponent, int adjustHp){
	CMDState* cmdState = new CMDState();
	cmdState->_pfComponent = pfComponent;
	cmdState->_adjustHp = adjustHp;
	cmdState->_timing = timing;
	return cmdState;
}

bool CMDState::execute(){
	CCLOG(">> CMDState::execute()");
	CC_ASSERT(_pfComponent != nullptr);
	if (_timing > 0 && _timing != BattleModel::getModel()->getCountdown())
		return false;
	_pfComponent->AddHp(_adjustHp);

	if (CMDFileStream::getInstance()->isStreamOn()){
		char buffer[1000];
		auto &pcp = _pfComponent;
		if (dynamic_cast<Troop*>(pcp) != nullptr){
			sprintf(buffer, "%d CMDState troop %s %d %d\n", BattleModel::getModel()->getCountdown(), pcp->getOwner().c_str(), pcp->getOID(), _adjustHp);
			CMDFileStream::getInstance()->append(buffer);
		}
		else if (dynamic_cast<Building*>(pcp) != nullptr){
			sprintf(buffer, "%d CMDState building %s %d %d\n", BattleModel::getModel()->getCountdown(), pcp->getOwner().c_str(), pcp->getOID(), _adjustHp);
			CMDFileStream::getInstance()->append(buffer);
		}
	}
	return true;
}
