#include "MapLayer.h"
#include "HUDLayer.h"
#include "MapModel.h"
#include "Building.h"
#include "SceneManager.h"
#include "PlayerManager.h"
#include "AppMacro.h"
#include "PlayerModel.h"
#include "BattleModel.h"
#include <ctime>
#include "Command.h"

USING_NS_CC;

MapLayer::MapLayer(){
	CCLOG("Map Layer construct");
	mm = MapModel::getModel();
	mm->Attach(this);
}

MapLayer::~MapLayer(){
	CCLOG("Map Layer destruct");
	mm->Detach(this);
	mm = nullptr;
	removeAllChildren();
}

void MapLayer::Update(Subject* _subject){
	setPosition(mm->getMapBasePosition());
}

MapLayer* MapLayer::create(std::string& mapName){
	MapLayer *pML = new MapLayer();
	if (pML && pML->init(mapName)) {
		pML->autorelease();
		return pML;
	}
	CC_SAFE_DELETE(pML);
	return NULL;
}

bool MapLayer::init(std::string& mapName){
	if (!BaseLayer::init()) return false;
	mm->init(mapName);
	mm->setMapBasePosition(getPosition());
	mm->setMapContent(getContentSize());
	double tstart = clock();

	auto _rangeSprites = Node::create();
	addChild(_rangeSprites, 5);
	mm->setRangeSprites(_rangeSprites);

	auto _tileMap = TMXTiledMap::create(mapName);
    CC_ASSERT(_tileMap!=nullptr);
	addChild(_tileMap, 0);
	mm->setTileMap(_tileMap);

	auto _pfLayers = mm->getPFLayers();
	mm->loadLayers(_pfLayers, "PF Layer");
	mm->setPFLayers(_pfLayers);
	CCLOG(">> %d\n", SZ(_pfLayers));

	mm->readMapInfo();

	double tend = clock();
	double tcost = (double)(tend - tstart) / CLOCKS_PER_SEC;
	CCLOG(">>>>>> TIME NEED %lf\n", tcost);
	
	//@debug conveniently use keyboard to restart in Win32.
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(MapLayer::keyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(MapLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MapLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MapLayer::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	this->scheduleUpdate();
	this->schedule(schedule_selector(MapLayer::refresh), mm->REFRESH_RATE);
	this->schedule(schedule_selector(MapLayer::ccdebug), 5.0f);

	return true;
}

void MapLayer::keyPressed(EventKeyboard::KeyCode keyCode, Event *event){
	SceneManager::pressKeyCode(keyCode);
	if (keyCode == EventKeyboard::KeyCode::KEY_A){
		BattleModel::getModel()->setupBattle(mm, true);
		BattleModel::getModel()->startBattle();
		this->schedule(schedule_selector(MapLayer::attack), 0.2f);
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_Z){
		this->unschedule(schedule_selector(MapLayer::attack));
		CMDFileStream::getInstance()->execute();
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_C)
		PlayerManager::getInstance()->getCurPlayer()->changeUID("1413977890666");

	if (keyCode == EventKeyboard::KeyCode::KEY_V)
		PlayerManager::getInstance()->getCurPlayer()->changeUID("1413977939022");
}

bool MapLayer::onTouchBegan(Touch *touch, Event *event){
	mm->setTouchLocationInGameLayer(convertTouchToNodeSpace(touch));
	mm->tryTouchBegan();
	return true;
}

void MapLayer::onTouchMoved(Touch* touch, Event* event){
	mm->setTouchLocationInGameLayer(convertTouchToNodeSpace(touch));
	mm->tryTouchMoved();
}

void MapLayer::onTouchEnded(Touch* touch, Event* event){
	mm->setTouchLocationInGameLayer(mm->getSelGroups()->getPosition() - getPosition());
	if (!mm->outsideBordor(getContentSize(), touch->getLocation()))
		mm->setTouchLocationInGameLayer(convertTouchToNodeSpace(touch));
	mm->tryTouchEnded();
}

void MapLayer::refresh(float dt){
	mm->refresh(dt);
}

void MapLayer::ccdebug(float dt){
	mm->ccdebug(dt);
}

void MapLayer::attack(float dt){
	CMDCountdown::order(BattleModel::getModel())->execute();
}