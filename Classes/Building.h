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
	static Building* build(int BID);

	void attackLogic();
	void replayLogic();

	CC_SYNTHESIZE(int, _range, Range);
	CC_SYNTHESIZE(Sprite*, _selSpriteRange, SelSpriteRange);	
};
