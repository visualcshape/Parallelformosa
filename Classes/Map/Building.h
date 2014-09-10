#pragma once
#include "cocos2d.h"
#include "AppMacro.h"

USING_NS_CC;

class Building : public Sprite {
public:
	CREATE_FUNC(Building);
	virtual bool init();

	int id;
	int range;
	int height;
	PII occupy;
	Sprite* sprite;
	Sprite* selSpriteRange;

	CC_SYNTHESIZE(Point, _coord, Coord);
	CC_SYNTHESIZE(Point, screenPosition, ScreenPosition);

private:
};

class OrangeGem : public Building {
public:
	static Building* build();
	virtual bool init();
};

class BlueGem : public Building {
public:
	static Building* build();
	virtual bool init();
};


class GreenGem : public Building {
public:
	static Building* build();
	virtual bool init();
};

class Star : public Building {
public:
	static Building* build();
	virtual bool init();
};