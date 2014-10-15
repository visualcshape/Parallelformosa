#include "command.h"

USING_NS_CC;

CMDState::CMDState(){

}

CMDState::~CMDState(){

}

void CMDState::init(){
}

CMDState* CMDState::order(PFComponent* pfComponent, int adjustHp){
	CMDState* cmdState = new CMDState();
	cmdState->_pfComponent = pfComponent;
	cmdState->_adjustHp = adjustHp;
	return cmdState;
}

void CMDState::execute(){
	CCLOG("CMDState::execute()");
	CC_ASSERT(_pfComponent != nullptr);
	_pfComponent->AddHp(_adjustHp);

	if (CMDFileStream::getInstance()->isStreamOn()){
		char buffer[1000];
		auto &pcp = _pfComponent;
		if (dynamic_cast<Troop*>(pcp) != nullptr){
			sprintf(buffer, "CMDState troop %d %d %d\n", pcp->getOwner(), pcp->getOID(), _adjustHp);
			CMDFileStream::getInstance()->append(buffer);
		}
		else if (dynamic_cast<Building*>(pcp) != nullptr){
			sprintf(buffer, "CMDState building %d %d %d\n", pcp->getOwner(), pcp->getOID(), _adjustHp);
			CMDFileStream::getInstance()->append(buffer);
		}
	}
}
