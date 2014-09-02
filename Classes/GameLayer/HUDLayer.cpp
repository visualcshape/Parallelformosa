#include "GameLayer\HUDLayer.h"
#include "GameLayer\MapLayer.h"
#include "Model\DataModel.h"
#include "DialogueWindowConfirm.h"
#include "AppMacro.h"
#include "VisibleRect.h"

USING_NS_CC;

HUDLayer* HUDLayer::_sharHUD;

HUDLayer::HUDLayer(){

	/*DialogueWindowConfirm* pDialogue = DialogueWindowConfirm::create("Error", Color3B(184, 41, 47), "Fail to connect to server", Color3B::BLACK);
	addChild(pDialogue, 100, "Dialogue");
	std::function<void(Ref*, ui::Widget::TouchEventType)> callback = [=](Ref* pSender, ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED){
			this->removeChildByName("Dialogue");
			pDialogue->autorelease();
		}
	};
	pDialogue->addButtonListener(callback);*/
}

HUDLayer::~HUDLayer(){
}

bool HUDLayer::init(){
	if (!BaseLayer::init())
		return false;

	Size winSize = CCDirector::getInstance()->getWinSize();

	// Draw the background of the game HUD
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGB565);
	background = Sprite::create("hud.png");
	background->setScaleX(2);
	background->setAnchorPoint(ccp(0, 0));
	this->addChild(background);
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_Default);

	selSprite = NULL;
	// Load the images of the towers we'll have and draw them to the game HUD layer
	Vector<String*> images;
	images.pushBack(StringMake("MachineGunTurret.png"));
	images.pushBack(StringMake("MachineGunTurret.png"));
	images.pushBack(StringMake("MachineGunTurret.png"));
	images.pushBack(StringMake("MachineGunTurret.png"));
	for (int i = 0; i < images.size(); ++i){
		String* image = images.at(i);
		auto *sprite = Sprite::create(image->getCString());
		float offsetFraction = ((float)(i + 1)) / (images.size() + 1);
		sprite->setPosition(ccp(winSize.width*offsetFraction, 35));
		this->addChild(sprite);
		movableSprites.pushBack(sprite);
	}

	//@var used to show message.
	TTFConfig config("fonts/Avenir.ttf", computeFontSize(8 * 4));
	Label* lblcursorPos = Label::createWithTTF(config, "x:?? y:??", TextHAlignment::LEFT);
	//lbl_cursorPos->setAnchorPoint(ccp(0, 0));
	lblcursorPos->setPosition(Vec2(VisibleRect::getVisibleRect().origin.x + 100, VisibleRect::getVisibleRect().size.height - 20));
	this->addChild(lblcursorPos, -1);
	this->setlblCursorPos(lblcursorPos);

	// used to manage building range images.
	rangeSprites = Node::create();
	this->addChild(rangeSprites);

	return true;
}

HUDLayer* HUDLayer::shareHUD(){
	if (_sharHUD == NULL)
		_sharHUD = HUDLayer::create();
	return _sharHUD;
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
	for each(Sprite* sprite in this->movableSprites){
		Rect pos_rect = Rect((sprite->getPositionX() - sprite->getContentSize().width / 2), (sprite->getPositionY() - sprite->getContentSize().height / 2), sprite->getContentSize().width, sprite->getContentSize().height);
		float xMin = pos_rect.getMinX();
		float xMax = pos_rect.getMaxX();
		float yMIn = pos_rect.getMinY();
		float yMax = pos_rect.getMaxY();
		if (pos_rect.containsPoint(touchLocation)){
			selSpriteRange = Sprite::create("Range.png");
			selSpriteRange->setScale(4);
			rangeSprites->addChild(selSpriteRange, -1, "range");
			selSpriteRange->setPosition(sprite->getPosition());

			newSprite = Sprite::createWithTexture(sprite->getTexture()); //sprite;
			newSprite->setPosition(sprite->getPosition());
			selSprite = newSprite;
			this->addChild(newSprite);

			for each(Tower* tower in m->getTowers()){
				auto rangeSprite = Sprite::create("Range.png");
				rangeSprite->setScale(4);
				rangeSprite->setPosition(tower->getPosition() + m->getGameLayer()->getPosition());
				rangeSprites->addChild(rangeSprite, -1);
			}
		}
	}
	return true;
}

void HUDLayer::onTouchMoved(Touch* touch, Event* event){

	//@remind modify debug label.
	DataModel *m = DataModel::getModel();
	char buffer[30];
	sprintf(buffer, "x:G+, y:G-");
	m->getMyHUDLayer()->getlblCursorPos()->setString(buffer);

	//avoid border error
	if (touch->getLocation().x < 0) return;
	if (touch->getLocation().x > m->getGameLayer()->getContentSize().width) return;
	if (touch->getLocation().y < 0) return;
	if (touch->getLocation().y > m->getGameLayer()->getContentSize().height) return;
	Point touchLocation = this->convertTouchToNodeSpace(touch);
	Point oldTouchLocation = this->convertToNodeSpace(touch->getPreviousLocation());
	Point translation = touchLocation - oldTouchLocation;

	if (selSprite){
		Point newPos = selSprite->getPosition() + translation;
		selSprite->setPosition(newPos);
		selSpriteRange->setPosition(newPos);

		Point touchLocationInGameLayer = m->getGameLayer()->convertTouchToNodeSpace(touch);

		BOOL isBuildable = m->getGameLayer()->canBuildOnTilePosition(touchLocationInGameLayer);
		if (isBuildable)
			selSprite->setOpacity(200);
		else
			selSprite->setOpacity(50);
	}
	else{
		Point oldBaseLayerLocation = m->getGameLayer()->getPosition();
		m->getGameLayer()->panForTranslation(translation);
		Point HUDtranstion = m->getGameLayer()->getPosition() - oldBaseLayerLocation;
		this->panForTranslation(HUDtranstion);
	}
}

void HUDLayer::onTouchEnded(Touch* touch, Event* event){
	//avoid border error
	DataModel *m = DataModel::getModel();
	if (touch->getLocation().x < 0) return;
	if (touch->getLocation().x > m->getGameLayer()->getContentSize().width) return;
	if (touch->getLocation().y < 0) return;
	if (touch->getLocation().y > m->getGameLayer()->getContentSize().height) return;

	Point touchLocation = this->convertTouchToNodeSpace(touch);

	if (selSprite){
		Rect backgroundRect = Rect(background->getPositionX(),
			background->getPositionY(),
			background->getContentSize().width,
			background->getContentSize().height);

		Point touchLocationInGameLayer = m->getGameLayer()->convertTouchToNodeSpace(touch);
		if (!backgroundRect.containsPoint(touchLocation) && m->getGameLayer()->canBuildOnTilePosition(touchLocationInGameLayer))
			m->getGameLayer()->addTower(touchLocationInGameLayer);

		rangeSprites->removeAllChildren();

		this->removeChild(selSprite, true);
		selSprite = NULL;
		this->removeChild(selSpriteRange, true);
		selSpriteRange = NULL;
	}
}

void HUDLayer::panForTranslation(Point translation){
	Point newPos = this->getPosition() - translation;
	this->setPosition(newPos);
}
