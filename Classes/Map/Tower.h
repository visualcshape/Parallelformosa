#pragma once
#include "cocos2d.h"

USING_NS_CC;

class Tower : public Sprite {
public:
	CC_SYNTHESIZE(Point, _coord, Coord);
	CC_SYNTHESIZE(Point, screenPosition, ScreenPosition);
	int range;
	Sprite* sprite;
	Sprite* selSpriteRange;
	CREATE_FUNC(Tower);
};

class MachineGunTower : public Tower {
public:
	static Tower* tower();
	bool virtual init();
};