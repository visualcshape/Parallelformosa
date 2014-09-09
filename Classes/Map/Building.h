#pragma once
#include "cocos2d.h"

USING_NS_CC;

class Building : public Sprite {
public:
	CREATE_FUNC(Building);

	int range;
	int height;
	Sprite* sprite;
	Sprite* selSpriteRange;

	CC_SYNTHESIZE(Point, _coord, Coord);
	CC_SYNTHESIZE(Point, screenPosition, ScreenPosition);

private:
};

class OrangeGem : public Building {
public:
	static Building* build(int level);
	virtual bool init();
};

class BlueGem : public Building {
public:
	static Building* build(int level);
	virtual bool init();
};


class GreenGem : public Building {
public:
	static Building* build(int level);
	virtual bool init();
};

class Star : public Building {
public:
	static Building* build(int level);
	virtual bool init();
};