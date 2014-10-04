#pragma once
#include "cocos2d.h"
#include "AppMacro.h"
#include "BaseLayer.h"
#include "Building.h"
#include "Subject.h"
#include "Observer.h"
#include "MapModel.h"

USING_NS_CC;

class BattleOverLayer : public LayerColor{
public:
	BattleOverLayer();
	~BattleOverLayer();

	static BattleOverLayer* create(std::string strLabel);
	bool init(std::string strLabel);

	void battleOverDone();

	CC_SYNTHESIZE_READONLY(LabelTTF*, _label, Label);

};