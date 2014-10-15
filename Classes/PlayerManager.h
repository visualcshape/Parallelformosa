#pragma once
#include "cocos2d.h"
#include "AppMacro.h"
#include "PlayerModel.h"

USING_NS_CC;

class PlayerManager :public Ref{
public:
	PlayerManager();
	~PlayerManager();

	//static PlayerModel* changePlayer(int uid);


private:
	static PlayerModel* _curPlayer;
	static PlayerModel* _atkPlayer;
	static PlayerModel* _defPlayer;
};