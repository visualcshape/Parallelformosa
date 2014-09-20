#pragma once
#include "cocos2d.h"

USING_NS_CC;

class Tower : public Sprite {
public:
	//@remind The access permission will modify when class "building" will be established.
	CREATE_FUNC(Tower);

	int range;
	Sprite* sprite;
	Sprite* selSpriteRange;

	CC_SYNTHESIZE(Point, _coord, Coord);
	CC_SYNTHESIZE(Point, screenPosition, ScreenPosition);
private:
};

class MachineGunTower : public Tower {
public:
	static Tower* tower();
	virtual bool init();
};
