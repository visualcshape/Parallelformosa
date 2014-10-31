#include "HUDLayer.h"
#include "MapLayer.h"
#include "ResourceModel.h"
#include "ResourceModel.h"
#include "MapModel.h"
#include "DialogueWindowConfirm.h"
#include "SceneManager.h"
#include "VisibleRect.h"
#include "AppMacro.h"
#include "PlayerManager.h"

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

HUDLayer* HUDLayer::create(HUD_ID status){
	HUDLayer *pHUD = new HUDLayer();
	if (pHUD && pHUD->init(status)) {
		pHUD->autorelease();
		return pHUD;
	}
	CC_SAFE_DELETE(pHUD);
	return NULL;
}

bool HUDLayer::init(HUD_ID status){
	if (!BaseLayer::init())
		return false;
    auto player = PlayerManager::getInstance()->getCurPlayer();
    
    
	mm->setHUDBasePosition(getPosition());
	mm->setStatus(status);
	Size winSize = CCDirector::getInstance()->getWinSize();
	
	// Load the images of the buildings we'll have and draw them to the game HUD layer
	ResourceModel *rm = ResourceModel::getModel();
	auto _moveableSprites = mm->getMovableSprites();
	if (mm->getStatus() == ATTACK){
		for (int i = 0; i < mm->BAR_ICON; ++i){
			auto sprite = Sprite::create(rm->strTroop[i + 1]);
			float offsetFraction = ((float)(i + 1)) / (mm->BAR_ICON + 1);
			sprite->setPosition(Point(winSize.width*offsetFraction, 70));
            TTFConfig ttfConfig("fonts/Silom.ttf",computeFontSize(20));
            Label* count = Label::createWithTTF(ttfConfig, to_string(player->getPlayerOwnedArcher()));
            
            
			addChild(sprite, 1);
			_moveableSprites.pushBack(sprite);
		}
		// Draw the background of the game HUD
		CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGB565);
		auto _background = Sprite::create("UI/HUD.png");
		_background->setScaleX(2);
		_background->setAnchorPoint(Point(0, 0));
		CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_Default);
		addChild(_background, 0);
		mm->setBackground(_background);
	}
	mm->setMovableSprites(_moveableSprites);

	
	mm->setSelSprite(nullptr);

	//@var used to show message.
	TTFConfig config("fonts/Avenir.ttf", computeFontSize(8 * 4));

	/*
	Label* lblcursorPos = Label::createWithTTF(config, "x:?? y:??", TextHAlignment::LEFT);
	lblcursorPos->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x + 200, VisibleRect::getVisibleRect().size.height - 20));
	addChild(lblcursorPos, -1);
	mm->setlblCursorPos(lblcursorPos);

	Label* lblTilePos = Label::createWithTTF(config, "row:?? col:??", TextHAlignment::LEFT);
	lblTilePos->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x + 200, VisibleRect::getVisibleRect().size.height - 60));
	addChild(lblTilePos, -1);
	mm->setlblTilePos(lblTilePos);

	Label* lblPlayerPos = Label::createWithTTF(config, "palyer uid=??", TextHAlignment::LEFT);
	lblPlayerPos->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x + 200, VisibleRect::getVisibleRect().size.height - 100));
	addChild(lblPlayerPos, -1);
	mm->setlblPlayerPos(lblPlayerPos);

	Label* lblResourcePos = Label::createWithTTF(config, "str = ??,\nmag = ??,\n food = ??,", TextHAlignment::LEFT);
	lblResourcePos->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x + 800, VisibleRect::getVisibleRect().size.height - 60));
	addChild(lblResourcePos, -1);
	mm->setlblResourcePos(lblResourcePos);
	*/
	Label* lblCountdownPos = Label::createWithTTF(config, "time remain: ??", TextHAlignment::LEFT);
	lblCountdownPos->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x + 500, VisibleRect::getVisibleRect().size.height - 60));
	addChild(lblCountdownPos, -1);
	mm->setlblCountdownPos(lblCountdownPos);
   
    
	//@var used to manage building range images.
	auto selGroups = Node::create();
	addChild(selGroups, -50);
	mm->setSelGroups(selGroups);

	scheduleUpdate();

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
	if (!mm->outsideBordor(getContentSize(), touch->getLocation()))
		mm->setTouchLocation(convertTouchToNodeSpace(touch));
}
