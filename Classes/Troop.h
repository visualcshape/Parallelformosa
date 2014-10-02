#pragma once
#include "cocos2d.h"
#include "AppMacro.h"
#include "PFComponent.h"
#include "Building.h"

USING_NS_CC;

class Troop : public PFComponent {
public:
	CREATE_FUNC(Troop);
	virtual bool init();

	int atk;
	int def;

	static Troop* addTroop(int TID);
	void goLeft();
	void goDown();
	void goRight();
	void goUp();
};