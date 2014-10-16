#include "command.h"
#include "MapModel.h"

USING_NS_CC;

CMDMove::CMDMove(){

}

CMDMove::~CMDMove(){

}

void CMDMove::init(){

}

CMDMove* CMDMove::order(int timing, Troop* troop, int dir, int hofs){
	CMDMove* cmdMove = new CMDMove();
	cmdMove->_troop = troop;
	cmdMove->_dir = dir;
	cmdMove->_hofs = hofs;
	cmdMove->_timing = timing;
	return cmdMove;
}

bool CMDMove::execute(){
	CCLOG(">> CMDMove::execute()");
	if (_timing > 0 && _timing != BattleModel::getModel()->getCountdown())
		return false;
	if (_dir == NO)
		return true;
	MapModel *mm = MapModel::getModel();
	mm->setTileGID(_troop->getCoord(), _troop->getZ(), _troop->getOccupy().X, _troop->getOccupy().Y, mm->EMPTY_TILE);
	_troop->go(_dir, _hofs);
	mm->setTileGID(_troop->getCoord(), _troop->getZ(), _troop->getOccupy().X, _troop->getOccupy().Y, _troop->getID());

	if (CMDFileStream::getInstance()->isStreamOn()){
		char buffer[1000];
		sprintf(buffer, "%d CMDMove troop %d %d %d %d\n", BattleModel::getModel()->getCountdown(), _troop->getOwner(), _troop->getOID(), _dir, _hofs);
		CMDFileStream::getInstance()->append(buffer);
	}
	return true;
}
