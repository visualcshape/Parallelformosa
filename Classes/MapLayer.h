#pragma once
#include "cocos2d.h"
#include "BaseLayer.h"
#include "Building.h"
#include "MapModel.h"
#include "Subject.h"
#include "Observer.h"

USING_NS_CC;

class MapLayer : public BaseLayer, public Observer{

public:
	MapLayer();
	~MapLayer();

	virtual void Update(Subject* _subject);

	virtual void keyPressed(EventKeyboard::KeyCode keyCode, Event *event);

	virtual bool onTouchBegan(Touch *touch, Event *event);
	virtual void onTouchMoved(Touch *touch, Event *event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	static MapLayer* create(std::string mapName);
	bool init(std::string mapName);

	void slide(Vec2 translation);
	void refresh(float dt);

protected:

private:
	MapModel *mm;
};
