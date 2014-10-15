#pragma once
#include "cocos2d.h"
#include "CMD.h"
#include "BattleModel.h"

USING_NS_CC;

class CMDCountdown :public CMD{

public:
	CMDCountdown();
	~CMDCountdown();

	static CMDCountdown* order(BattleModel* battle);
	void init();
	virtual void execute();

private:
	BattleModel* _battle;
};
