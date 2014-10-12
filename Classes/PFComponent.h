#pragma once
#include "cocos2d.h"
#include "AppMacro.h"

USING_NS_CC;

class PFComponent : public Sprite {
public:
	PFComponent();
	~PFComponent();

	CREATE_FUNC(PFComponent);
	virtual bool init();

	int id;
	int owner; //uid
	int height;
	PII occupy;
	Sprite* sprite;

	int atk;
	int def;
	int hp;

	CC_SYNTHESIZE(Point, _coord, Coord);
	//CC_SYNTHESIZE(Point, screenPosition, ScreenPosition);

};
