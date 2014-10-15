#include "command.h"
#include "MapModel.h"

USING_NS_CC;

CMDMove::CMDMove(){

}

CMDMove::~CMDMove(){

}

void CMDMove::init(){

}

CMDMove* CMDMove::order(Troop* troop, int dir, int hofs){
	CMDMove* cmdMove = new CMDMove();
	cmdMove->_troop = troop;
	cmdMove->_dir = dir;
	cmdMove->_hofs = hofs;
	return cmdMove;
}

void CMDMove::execute(){
	CCLOG("CMDMove::execute()");
	if (_dir == NO) return;
	MapModel *mm = MapModel::getModel();
	mm->setTileGID(_troop->getCoord(), _troop->getZ(), _troop->getOccupy().X, _troop->getOccupy().Y, mm->EMPTY_TILE);
	_troop->go(_dir, _hofs);
	mm->setTileGID(_troop->getCoord(), _troop->getZ(), _troop->getOccupy().X, _troop->getOccupy().Y, _troop->getID());

	if (CMDFileStream::getInstance()->isStreamOn()){
		char buffer[1000];
		sprintf(buffer, "CMDMove troop %d %d %d %d\n", _troop->getOwner(), _troop->getOID(), _dir, _hofs);
		CMDFileStream::getInstance()->append(buffer);
	}
}
