#pragma once
#include "cocos2d.h"
#include "GameLayer\BaseLayer.h"

USING_NS_CC;

class HUDLayer : public BaseLayer{
public:
	HUDLayer();
	~HUDLayer();

	virtual bool init();

	CREATE_FUNC(HUDLayer);

	virtual void onEnter();
	virtual bool onTouchBegan(Touch *touch, Event *event);
	virtual void onTouchMoved(Touch *touch, Event *event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	void refresh(float dt);

	CC_SYNTHESIZE(Label*, _lblCursorPos, lblCursorPos);

protected:
	void panForTranslation(Point translation);
	bool outsideBordor(Touch* touch);
	void slide(Vec2 translation);

private:
	Sprite* background;
	Sprite* selSpriteRange;
	Sprite* selSprite;
	Vector <Sprite*> movableSprites;
	Node* selGroups;

	bool prevCursurOutside;
};

