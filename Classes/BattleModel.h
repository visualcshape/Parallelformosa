#pragma once
#include "cocos2d.h"
#include "Building.h"
#include "Troop.h"

USING_NS_CC;

class BattleModel :public Ref{

public:
	BattleModel();
	~BattleModel();

	static BattleModel* getModel();

	void sendRequest(Vector <Building*> buildings);
	void sendRequest(Vector <Troop*> troops);



protected:
	static BattleModel* bm_pInstance;
};
