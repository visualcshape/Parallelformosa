#pragma once
#include "cocos2d.h"
#include "BaseLayer.h"
#include "Building.h"
#include "Subject.h"
#include "Observer.h"
#include "MapModel.h"

USING_NS_CC;

class HUDLayer : public BaseLayer, public Observer{

public:
	HUDLayer();
	~HUDLayer();

	virtual bool init();

	virtual void Update(Subject* _subject);
	CREATE_FUNC(HUDLayer);

	virtual bool onTouchBegan(Touch *touch, Event *event);
	virtual void onTouchMoved(Touch *touch, Event *event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	void panForTranslation(Point translation);		

private:
	MapModel *mm;
};

