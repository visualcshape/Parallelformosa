#pragma once
#include "cocos2d.h"
#include "GameLayer\BaseLayer.h"
#include "Map\Building.h"

USING_NS_CC;

class HUDLayer : public BaseLayer{
	const float SLIDE_RATE = 25.0f;
	static const int BORDER_PIXEL = 40;
	static const int BAR_ICON = 5;

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
	CC_SYNTHESIZE(Label*, _lblTilePos, lblTilePos);
	CC_SYNTHESIZE(Label*, _lblBuldingPos, lblBuldingPos);

protected:
	void panForTranslation(Point translation);
	bool outsideBordor(Touch* touch);
	void slide(Vec2 translation);

private:
	Sprite* background;
	Sprite* selSpriteRange;
	Sprite* selSprite;
	Point pressLoc;
	int selID;

	Vector <Sprite*> movableSprites;

	Node* selGroups;

	bool prevCursurOutside;
};

