#pragma once
#include "cocos2d.h"
#include "Building.h"
#include "Troop.h"
#include "PlayerModel.h"
#include "MapModel.h"

USING_NS_CC;

class BattleModel :public Ref{

public:
	BattleModel();
	~BattleModel();

	static BattleModel* getModel();

	void setupBattle(PlayerModel* atkPlayer, PlayerModel* defPlayer, MapModel* mapModel);
	void simulateBattle();
	void doBattle();

	void sendRequest(Vector <Building*> buildings);
	void sendRequest(Vector <Troop*> troops);
	
	void timePass();
	void battleOver(PlayerModel* winPlayer);

	CC_SYNTHESIZE(int, _countdown, Countdown);
	CC_SYNTHESIZE(PlayerModel*, _atkPlayer, AtkPlayer);
	CC_SYNTHESIZE(PlayerModel*, _defPlayer, DefPlayer);

protected:
	static BattleModel* bm_pInstance;

private:
	MapModel* _mapModel;
};
