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

	void setupBattle(MapModel* mapModel, bool isSimulation);
	void startBattle();

	void sendRequest(Vector <Building*> buildings);
	void sendRequest(Vector <Troop*> troops);
	
	void timePass();
	void battleOver(PlayerModel* winPlayer);

	CC_SYNTHESIZE(int, _countdown, Countdown);

protected:
	static BattleModel* bm_pInstance;
	void readCommandConfig();

private:
	MapModel* _mapModel;
	bool _isSimulation;
	bool _isReplay;
};
