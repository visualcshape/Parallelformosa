#include "TrademarkLayer.h"
#include "VisibleRect.h"

USING_NS_CC;

TrademarkLayer::TrademarkLayer(){
}

TrademarkLayer::~TrademarkLayer(){
}

bool TrademarkLayer::init(){
	if (!BaseLayer::init()) return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//?
	m_tradeMark = Sprite::create("Trademark/Mark.png");
	CC_ASSERT(m_tradeMark != NULL);
	m_tradeMark->setAnchorPoint(Vec2(0.5, 0.5));
	m_tradeMark->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	m_tradeMark->setOpacity(0);

	//A trabsparentSprite
	auto transparentSprite = Sprite::create();
	CC_ASSERT(transparentSprite != nullptr);
	transparentSprite->setTextureRect(Rect(VisibleRect::getVisibleRect().origin.x, VisibleRect::getVisibleRect().origin.y, VisibleRect::getVisibleRect().size.width, VisibleRect::getVisibleRect().size.height));

	//Add transparent sprite as menu item
	m_transparentSpriteMenuItem = MenuItemSprite::create(transparentSprite, NULL);
	CC_ASSERT(m_transparentSpriteMenuItem != NULL);
	m_transparentSpriteMenuItem->setCallback(CC_CALLBACK_1(TrademarkLayer::back, this));
	m_transparentSpriteMenuItem->setOpacity(0);

	//Menu for transparents sprite
	m_menu = Menu::create(m_transparentSpriteMenuItem, NULL);
	CC_ASSERT(m_menu != nullptr);
	addChild(m_menu, 0);

	addChild(m_tradeMark, 1);

	//Call animation
	animate();

	//Console : Output Size
	CCLOG("%f, %f", visibleSize.width, visibleSize.height);

	return true;
}

void TrademarkLayer::animate(){
	animateTrademark();
}

void TrademarkLayer::animateTrademark(){
	float timeToFadeIn = 1.0f;
	float timeToFadeOut = 2.0f;

	auto fadeIn = FadeIn::create(timeToFadeIn);
	auto fadeOut = FadeOut::create(timeToFadeOut);

	auto callFunc = CallFuncN::create(CC_CALLBACK_1(TrademarkLayer::back, this));

	auto sequence = Sequence::create(fadeIn, DelayTime::create(2.0f), fadeOut, callFunc, NULL);

	m_tradeMark->runAction(sequence);
}

void TrademarkLayer::back(Object* pSender){
	SceneManager::goTitleScreen();
}