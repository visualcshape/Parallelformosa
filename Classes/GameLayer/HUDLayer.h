#pragma once
#include "cocos2d.h"
#include "GameLayer\BaseLayer.h"

USING_NS_CC;

class HUDLayer : public BaseLayer{
public:
	HUDLayer();
	~HUDLayer();

	virtual bool init();
	static HUDLayer* shareHUD();

	CREATE_FUNC(HUDLayer);

	virtual void onEnter();
	virtual bool onTouchBegan(Touch *touch, Event *event);
	virtual void onTouchMoved(Touch *touch, Event *event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	CC_SYNTHESIZE(Label*, _lblCursorPos, lblCursorPos);

protected:
	void panForTranslation(Point translation);

private:
	static HUDLayer* _sharHUD;

	Sprite* background;
	Sprite* selSpriteRange;
	Sprite* selSprite;
	Vector <Sprite*> movableSprites;
	Node* rangeSprites;

};

