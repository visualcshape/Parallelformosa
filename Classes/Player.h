#pragma once
#include "cocos2d.h"
#include "AppMacro.h"
#include "MapModel.h"

USING_NS_CC;

class Player : public Ref {
public:
	Player();
	~Player();

	CREATE_FUNC(Player);
	virtual bool init();

	void putBuilding();


private:
	MapModel *mm;
	int L_str;
	int G_mag;
};
