#pragma once
#include "cocos2d.h"
#include "AppMacro.h"

USING_NS_CC;

class Building : public Sprite {
public:
	CREATE_FUNC(Building);
	virtual bool init();

	int owner;
	int id;
	int range;
	int height;
	PII occupy;
	Sprite* sprite;
	Sprite* selSpriteRange;

	static Building* build(int BID);
	CC_SYNTHESIZE(Point, _coord, Coord);
	CC_SYNTHESIZE(Point, screenPosition, ScreenPosition);

};
