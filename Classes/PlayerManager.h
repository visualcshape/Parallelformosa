#pragma once
#include "cocos2d.h"
#include "AppMacro.h"
#include "PlayerModel.h"

USING_NS_CC;

class PlayerManager :public Ref{
public:
	PlayerManager();
	~PlayerManager();

	static PlayerManager* getInstance();

	CC_SYNTHESIZE(PlayerModel*, _curPlayer, CurPlayer);
	CC_SYNTHESIZE(PlayerModel*, _atkPlayer, AtkPlayer);
	CC_SYNTHESIZE(PlayerModel*, _defPlayer, DefPlayer);

private:
	static PlayerManager* pmr_pInstance;
};