#pragma once
#include "cocos2d.h"
#include "AppMacro.h"
#include "PFComponent.h"
#include "ResourceModel.h"

USING_NS_CC;

class Building : public PFComponent {
	
	
public:
	CREATE_FUNC(Building);
	virtual bool init();
	//static Building initWithBuilding(const Building *_building);

	int range;
	Sprite* selSpriteRange;

	static Building* build(int BID);

};
