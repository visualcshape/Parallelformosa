#include "HUDLayer.h"
#include "MapLayer.h"
#include "ResourceModel.h"
#include "DataModel.h"
#include "ResourceModel.h"
#include "MapModel.h"
#include "DialogueWindowConfirm.h"
#include "SceneManager.h"
#include "VisibleRect.h"
#include "AppMacro.h"

USING_NS_CC;

HUDLayer::HUDLayer(){
	CCLOG("HUD Layer construct");
	mm = MapModel::getModel();
	mm->Attach(this);
}

HUDLayer::~HUDLayer(){
	CCLOG("HUD Layer destruct");
	mm->Detach(this);
	mm = nullptr;
	removeAllChildren();
}

bool HUDLayer::init(){
	if (!BaseLayer::init())
		return false;

	mm->setHUDBasePosition(getPosition());

	Size winSize = CCDirector::getInstance()->getWinSize();

	// Load the images of the buildings we'll have and draw them to the game HUD layer
	ResourceModel *rm = ResourceModel::getModel();
	auto _moveableSprites = mm->getMovableSprites();
	for (int i = 0; i < mm->BAR_ICON; ++i){
		auto sprite = Sprite::create(rm->strBuilding[i + 1]);
		float offsetFraction = ((float)(i + 1)) / (mm->BAR_ICON + 1);
		sprite->setPosition(Point(winSize.width*offsetFraction, 70));
		addChild(sprite, 1);
		_moveableSprites.pushBack(sprite);
	}
	mm->setMovableSprites(_moveableSprites);

	// Draw the background of the game HUD
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGB565);
	auto _background = Sprite::create("hud.png");
	_background->setScaleX(2);
	_background->setAnchorPoint(Point(0, 0));
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_Default);
	addChild(_background, 0);
	mm->setBackground(_background);
	
	mm->setSelSprite(nullptr);

	//@var used to show message.
	TTFConfig config("fonts/Avenir.ttf", computeFontSize(8 * 4));
	Label* lblcursorPos = Label::createWithTTF(config, "x:?? y:??", TextHAlignment::LEFT);
	//lbl_cursorPos->setAnchorPoint(ccp(0, 0));
	lblcursorPos->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x + 200, VisibleRect::getVisibleRect().size.height - 20));
	addChild(lblcursorPos, -1);
	mm->setlblCursorPos(lblcursorPos);

	Label* lblTilePos = Label::createWithTTF(config, "row:?? col:??", TextHAlignment::LEFT);
	lblTilePos->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x + 200, VisibleRect::getVisibleRect().size.height - 60));
	addChild(lblTilePos, -1);
	mm->setlblTilePos(lblTilePos);

	Label* lblBuldingPos = Label::createWithTTF(config, "bulding id=??", TextHAlignment::LEFT);
	lblBuldingPos->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x + 200, VisibleRect::getVisibleRect().size.height - 100));
	addChild(lblBuldingPos, -1);
	mm->setlblBuldingPos(lblBuldingPos);

	//@var used to manage building range images.
	auto selGroups = Node::create();
	addChild(selGroups, -50);
	mm->setSelGroups(selGroups);

	scheduleUpdate();
	//schedule(schedule_selector(HUDLayer::refresh), 0.05f);
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);

	listener->onTouchBegan = CC_CALLBACK_2(HUDLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HUDLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HUDLayer::onTouchEnded, this);

	auto dispatcher = Director::getInstance()->getEventDispatcher();

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void HUDLayer::Update(Subject* _subject){
	setPosition(mm->getHUDBasePosition());
}

bool HUDLayer::onTouchBegan(Touch *touch, Event *event){
	mm->setTouchLocation(convertTouchToNodeSpace(touch));
	mm->setPressLoc(convertTouchToNodeSpace(touch));
	return true;
}

void HUDLayer::onTouchMoved(Touch* touch, Event* event){
	mm->setOldTouchLocation(convertToNodeSpace(touch->getPreviousLocation()));
	mm->setTouchLocation(convertTouchToNodeSpace(touch));

}

void HUDLayer::onTouchEnded(Touch* touch, Event* event){
	mm->setTouchLocation(mm->getSelGroups()->getPosition());
	if (!mm->outsideBordor(getContentSize(), touch->getLocation())){
		mm->setTouchLocation(convertTouchToNodeSpace(touch));
	}
}
