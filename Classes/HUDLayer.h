#pragma once
#include "cocos2d.h"
#include "AppMacro.h"
#include "BaseLayer.h"
#include "Building.h"
#include "Subject.h"
#include "Observer.h"
#include "MapModel.h"

USING_NS_CC;

class HUDLayer : public BaseLayer, public ScreenObserver{

public:
	HUDLayer();
	~HUDLayer();

	static HUDLayer* create(HUD_ID status);
	bool init(HUD_ID status);

	virtual void Update(Subject* _subject);

	virtual bool onTouchBegan(Touch *touch, Event *event);
	virtual void onTouchMoved(Touch *touch, Event *event);
	virtual void onTouchEnded(Touch* touch, Event* event);

private:
	MapModel *mm;
};

