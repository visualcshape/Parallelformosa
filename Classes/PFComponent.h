#pragma once
#include "cocos2d.h"
#include "AppMacro.h"

USING_NS_CC;

class PFComponent : public Sprite {
public:
	CREATE_FUNC(PFComponent);
	virtual bool init();

	int id;
	int owner;
	int height;
	PII occupy;
	Sprite* sprite;

	int atk;
	int def;
	int hp;

	CC_SYNTHESIZE(Point, _coord, Coord);
	CC_SYNTHESIZE(Point, screenPosition, ScreenPosition);

};
