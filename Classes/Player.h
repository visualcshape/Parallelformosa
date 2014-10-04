#pragma once
#include "cocos2d.h"
#include "AppMacro.h"

USING_NS_CC;

class Player : public Ref {
public:
	Player();
	~Player();

	void init(int uid);
	bool canAddTroop(int TID);
	void consumeResource(int TID);

	CC_SYNTHESIZE(int, _uid, Uid);

	int L_str;
	int G_mag;
	int food;

	
};
