#include "command.h"
#include "MapModel.h"

USING_NS_CC;

CMDResource::CMDResource(){
}

CMDResource::~CMDResource(){
}

CMDResource* CMDResource::order(PlayerModel* player, int Lstr, int Gmag, int food){
	CMDResource* cmdResource = new CMDResource();
	cmdResource->_player = player;
	cmdResource->_Lstr = Lstr;
	cmdResource->_Gmag = Gmag;
	cmdResource->_food = food;
	return cmdResource;
}

bool CMDResource::execute(){
	CCLOG("CMDResource::execute()");
	_player->gainLstr(_Lstr);
	_player->gainGmag(_Gmag);
	_player->gainFood(_food);
	return true;
}
