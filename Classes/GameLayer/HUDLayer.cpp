#include "GameLayer\HUDLayer.h"
#include "GameLayer\MapLayer.h"
#include "Model\ResourceModel.h"
#include "Model\DataModel.h"
#include "DialogueWindowConfirm.h"
#include "AppMacro.h"
#include "VisibleRect.h"

USING_NS_CC;

HUDLayer::HUDLayer(){
}

HUDLayer::~HUDLayer(){
	DataModel *m = DataModel::getModel();
	auto buildings = m->getBuildings(); buildings.clear(); m->setBuildings(buildings);
	m->setMyHUDLayer(NULL);
}

bool HUDLayer::init(){
	if (!BaseLayer::init())
		return false;

	Size winSize = CCDirector::getInstance()->getWinSize();

	DataModel *m = DataModel::getModel();
	m->setMyHUDLayer(this);

	// Draw the background of the game HUD
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGB565);
	background = Sprite::create("hud.png");
	background->setScaleX(2);
	background->setAnchorPoint(ccp(0, 0));
	this->addChild(background);
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_Default);

	selSprite = NULL;
	// Load the images of the buildings we'll have and draw them to the game HUD layer
	ResourceModel *rm = ResourceModel::getModel();

	for (int i = 0; i < BAR_ICON; ++i){
		auto *sprite = rm->picBuilding[i + 1];
		float offsetFraction = ((float)(i + 1)) / (BAR_ICON + 1);
		sprite->setPosition(ccp(winSize.width*offsetFraction, 70));
		this->addChild(sprite);
		movableSprites.pushBack(sprite);
	}

	//@var used to show message.
	TTFConfig config("fonts/Avenir.ttf", computeFontSize(8 * 4));
	Label* lblcursorPos = Label::createWithTTF(config, "x:?? y:??", TextHAlignment::LEFT);
	//lbl_cursorPos->setAnchorPoint(ccp(0, 0));
	lblcursorPos->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x + 200, VisibleRect::getVisibleRect().size.height - 20));
	this->addChild(lblcursorPos, -1);
	this->setlblCursorPos(lblcursorPos);

	Label* lblTilePos = Label::createWithTTF(config, "row:?? col:??", TextHAlignment::LEFT);
	lblTilePos->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x + 200, VisibleRect::getVisibleRect().size.height - 60));
	this->addChild(lblTilePos, -1);
	this->setlblTilePos(lblTilePos);

	Label* lblBuldingPos = Label::createWithTTF(config, "bulding id=??", TextHAlignment::LEFT);
	lblBuldingPos->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x + 200, VisibleRect::getVisibleRect().size.height - 100));
	this->addChild(lblBuldingPos, -1);
	this->setlblBuldingPos(lblBuldingPos);

	//@var used to manage building range images.
	selGroups = Node::create();
	this->addChild(selGroups, -50);

	prevCursurOutside = false;

	this->scheduleUpdate();
	this->schedule(schedule_selector(HUDLayer::refresh), 0.05f);
	return true;
}

void HUDLayer::onEnter(){
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HUDLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HUDLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HUDLayer::onTouchEnded, this);

	auto dispatcher = Director::getInstance()->getEventDispatcher();

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool HUDLayer::onTouchBegan(Touch *touch, Event *event){
	Point touchLocation = this->convertTouchToNodeSpace(touch);

	DataModel *m = DataModel::getModel();

	Sprite * newSprite = NULL;
	Building* target = NULL;
	for (int i = 0; i < movableSprites.size(); i++){
		Sprite* sprite = movableSprites.at(i);
		Rect pos_rect = Rect((sprite->getPositionX() - sprite->getContentSize().width / 2), (sprite->getPositionY() - sprite->getContentSize().height / 2), sprite->getContentSize().width, sprite->getContentSize().height);
		float xMin = pos_rect.getMinX();
		float xMax = pos_rect.getMaxX();
		float yMIn = pos_rect.getMinY();
		float yMax = pos_rect.getMaxY();
		if (pos_rect.containsPoint(touchLocation)){
			selSpriteRange = Sprite::create("Range.png");
			selSpriteRange->setScale(4);
			selGroups->setPosition(sprite->getPosition());
			selGroups->addChild(selSpriteRange);

			newSprite = Sprite::createWithTexture(sprite->getTexture()); //sprite;
			selSprite = newSprite;
			selGroups->addChild(newSprite);

			selID = i + 1;
			m->getGameLayer()->showAllRange(true);
		}
	}
	return true;
}

void HUDLayer::onTouchMoved(Touch* touch, Event* event){
	DataModel *m = DataModel::getModel();

	//avoid border error
	if (this->outsideBordor(touch)){
		prevCursurOutside = true;
		return;
	}

	Point touchLocation = this->convertTouchToNodeSpace(touch);
	Point oldTouchLocation = this->convertToNodeSpace(touch->getPreviousLocation());
	Point translation = touchLocation - oldTouchLocation;

	if (prevCursurOutside){
		selGroups->setPosition(touchLocation);
		prevCursurOutside = false;
	}

	if (selSprite){
		selGroups->setPosition(selGroups->getPosition() + translation);

		Point touchLocationInGameLayer = m->getGameLayer()->convertTouchToNodeSpace(touch);

		//@debug modify label.
		char buffer[30];
		sprintf(buffer, "x:%.1f, y:%.1f", touchLocationInGameLayer.x, touchLocationInGameLayer.y);
		m->getMyHUDLayer()->getlblCursorPos()->setString(buffer);

		int isBuildableLevel = m->getGameLayer()->canBuildOnTilePosition(touchLocationInGameLayer, selID);
		if (~isBuildableLevel){
			selSprite->setOpacity(200);
			m->getGameLayer()->setTileMark(touchLocationInGameLayer, selID, isBuildableLevel, true);
		}
		else{
			selSprite->setOpacity(50);
			m->getGameLayer()->removeTileMark();
			//m->getGameLayer()->setTileMark(touchLocationInGameLayer, selID, isBuildableLevel, false);
		}
	}
	else{
		this->slide(translation);
	}
}

void HUDLayer::onTouchEnded(Touch* touch, Event* event){
	DataModel *m = DataModel::getModel();

	if (selSprite){
		Rect backgroundRect = Rect(background->getPositionX(),
			background->getPositionY(),
			background->getContentSize().width,
			background->getContentSize().height);

		Point touchLocationInGameLayer = selGroups->getPosition() - m->getGameLayer()->getPosition();
		Point touchLocation = selGroups->getPosition();

		//avoid border error
		if (!this->outsideBordor(touch)){
			prevCursurOutside = false;
			touchLocationInGameLayer = m->getGameLayer()->convertTouchToNodeSpace(touch);
			touchLocation = this->convertTouchToNodeSpace(touch);
		}

		if (!backgroundRect.containsPoint(touchLocation)){
			int isBuildableLevel = m->getGameLayer()->canBuildOnTilePosition(touchLocationInGameLayer, selID);
			if (~isBuildableLevel)
				m->getGameLayer()->addBuilding(touchLocationInGameLayer, selID, isBuildableLevel);
		}

		selGroups->removeAllChildren();
		selSprite = NULL;
		selSpriteRange = NULL;

		m->getGameLayer()->showAllRange(false);
		m->getGameLayer()->removeTileMark();
	}
}

void HUDLayer::panForTranslation(Point translation){
	this->setPosition(this->getPosition() - translation);
}

bool HUDLayer::outsideBordor(Touch* touch){
	DataModel *m = DataModel::getModel();

	Point pos = touch->getLocation();

	//@remind use BORDER_PIXEL pixel to avoid border error
	if (pos.x < BORDER_PIXEL || pos.x + BORDER_PIXEL > m->getGameLayer()->getContentSize().width)
		return true;
	if (pos.y < BORDER_PIXEL || pos.y + BORDER_PIXEL > m->getGameLayer()->getContentSize().height)
		return true;

	return false;
}

void HUDLayer::slide(Vec2 translation){
	DataModel *m = DataModel::getModel();
	Point oldBaseLayerLocation = m->getGameLayer()->getPosition();
	m->getGameLayer()->panForTranslation(translation);
	Point HUDtranstion = m->getGameLayer()->getPosition() - oldBaseLayerLocation;
	this->panForTranslation(HUDtranstion);
}

void HUDLayer::refresh(float dt){
	DataModel *m = DataModel::getModel();
	if (selSprite){
		if (selGroups->getPositionX() < BORDER_PIXEL)
			this->slide(Vec2(SLIDE_RATE, 0));
		if (selGroups->getPositionX() >= m->getGameLayer()->getContentSize().width - BORDER_PIXEL)
			this->slide(Vec2(-SLIDE_RATE, 0));
		if (selGroups->getPositionY() < BORDER_PIXEL)
			this->slide(Vec2(0, SLIDE_RATE));
		if (selGroups->getPositionY() >= m->getGameLayer()->getContentSize().height - BORDER_PIXEL)
			this->slide(Vec2(0, -SLIDE_RATE));
	}
}
