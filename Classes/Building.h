#pragma once
#include "cocos2d.h"
#include "AppMacro.h"
#include "PFComponent.h"
#include "ResourceModel.h"

USING_NS_CC;

class Building : public PFComponent {
public:
	Building();
	~Building();

	CREATE_FUNC(Building);
	virtual bool init();

	int range;
	Sprite* selSpriteRange;

	static Building* build(int BID);
	void attackLogic();

};
