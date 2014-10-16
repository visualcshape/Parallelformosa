#pragma once
#include "cocos2d.h"
#include "CMD.h"
#include "Troop.h"

USING_NS_CC;

class CMDMove :public CMD{

public:
	CMDMove();
	~CMDMove();

	static CMDMove* order(int timing, Troop* troop, int dir, int hofs);

	void init();
	virtual bool execute();

private:
	Troop* _troop;
	int _dir;
	int _hofs;
};
