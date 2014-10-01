#pragma once
#include "cocos2d.h"
#include "AppMacro.h"
#include "PFComponent.h"

USING_NS_CC;

class Building : public PFComponent {
public:
	CREATE_FUNC(Building);
	virtual bool init();

	int range;
	Sprite* selSpriteRange;

	static Building* build(int BID);

};
