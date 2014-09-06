#include "GameLayer\MapLayer.h"
#include "GameLayer\HUDLayer.h"
#include "Model\DataModel.h"
#include "SceneManager.h"

USING_NS_CC;

MapLayer::MapLayer(){

}

MapLayer::~MapLayer(){
	DataModel* m = DataModel::getModel();
	m->setGameLayer(NULL);
}

bool MapLayer::init(){
	if (!BaseLayer::init())
		return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();

	auto myHUDLayer = HUDLayer::create();
	myHUDLayer->setPosition(Vec2(visibleOrigin.x, visibleOrigin.y));
	this->addChild(myHUDLayer, 100);

	rangeSprites = Node::create();
	this->addChild(rangeSprites, 5);

	tileMark = Node::create();
	this->addChild(tileMark, 5);

	DataModel* m = DataModel::getModel();
	m->setGameLayer(this);

	this->tileMap = TMXTiledMap::create("TestMap.tmx");
	this->tileLayer = tileMap->layerNamed("Tile Layer 1");
	this->tileLayer->setAnchorPoint(ccp(0, 0));
	this->addChild(tileMap, 0);


	this->scheduleUpdate();

	//@debug conveniently use keyboard to restart in Win32.
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(MapLayer::keyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	return true;
}

Point MapLayer::tileCoordForPosition(Point position){
	int x = position.x / this->tileMap->getTileSize().width;
	int y = (this->tileMap->getMapSize().height *this->tileMap->getTileSize().height - position.y) / this->tileMap->getTileSize().height;

	return ccp(x, y);
}

Point MapLayer::mapCoordForPosition(Point position, int level){
	Point TileLoc = this->tileCoordForPosition(position);
	const int MH = this->tileMap->getMapSize().height;
	int row = MH -1-level - TileLoc.y;
	row = (row) >> 1;


	return ccp(TileLoc.x, row);
}

void MapLayer::addBuilding(Point pos, int id){
	DataModel *m = DataModel::getModel();
	Building *target = NULL;
	if (!this->canBuildOnTilePosition(pos)){
		CCLOG("Tile Not Buildable");
		return;
	}

	Point buildingLoc = this->mapCoordForPosition(pos, 1);

	//@debug modify label.
	//DataModel *m = DataModel::getModel();
	char buffer[30];
	sprintf(buffer, "row:%.0f, col:%.0f", buildingLoc.x, buildingLoc.y);
	m->getMyHUDLayer()->getlblTilePos()->setString(buffer);

	const int TW = this->tileMap->getTileSize().width;
	const int TH = 82;
	switch (id){
	case 0: target = OrangeGem::build(); break;
	case 1: target = BlueGem::build(); break;
	case 2: target = GreenGem::build(); break;
	case 3: target = Star::build(); break;
	}
	target->setAnchorPoint(ccp(0, 0));
	target->setPosition(ccp((buildingLoc.x * TW) + TW / 2, (buildingLoc.y * TH + 123)));
	target->setCoord(buildingLoc);
	this->addChild(target, 30-buildingLoc.y);
	Vector <Building*> V = m->getBuildings();	V.pushBack(target);	m->setBuildings(V);
	CCLOG("add building");
}

bool MapLayer::canBuildOnTilePosition(Point pos){ // set @param level to 1
	Point TileLoc = this->tileCoordForPosition(pos);
	Point buildingLoc = this->mapCoordForPosition(pos, 1);

	for (int offset = 1; offset <= 2; offset++){
		Point checkTileLoc = ccp(TileLoc.x, TileLoc.y + offset);
		CCLOG("checktile %.0f %.0f", checkTileLoc.x, checkTileLoc.y);
		if (checkTileLoc.y < 0) continue;
		if (checkTileLoc.y >= this->tileMap->getMapSize().height) continue;
		int tileGid = this->tileLayer->getTileGIDAt(checkTileLoc);

		DataModel* m = DataModel::getModel();
		for each(Building *building in m->getBuildings()){
			if (building->getCoord() == buildingLoc)
				return false;
		}

		Value props = this->tileMap->getPropertiesForGID(tileGid);
		if (!props.isNull()){
			ValueMap map = props.asValueMap();
			int type_int = 0;
			if (map.size() == 0)
				type_int = 0;
			else
				type_int = map.at("buildable").asInt();
			if (1 == type_int)
				return true;
		}
	}
	return false;
}

Point MapLayer::boundLayerPos(Point newPos){
	Size winSize = Director::getInstance()->getWinSize();
	Point retval = newPos;

	// inside the map, don't over border.
	retval.x = MIN(retval.x, 0);
	retval.x = MAX(retval.x, -tileLayer->getContentSize().width + winSize.width);

	retval.y = MIN(retval.y, 0);
	retval.y = MAX(retval.y, -tileLayer->getContentSize().height + winSize.height);

	return retval;
}

void MapLayer::panForTranslation(Point translation){
	Point newPos = this->getPosition() + translation;
	this->setPosition(this->boundLayerPos(newPos));
}

void MapLayer::keyPressed(EventKeyboard::KeyCode keyCode, Event *event){
	SceneManager::pressKeyCode(keyCode);
}

void MapLayer::showAllRange(bool visible){
	DataModel *m = DataModel::getModel();
	if (visible){
		for each(Building* building in m->getBuildings()){
			auto rangeSprite = Sprite::create("Range.png");
			rangeSprite->setScale(4);
			rangeSprite->setPosition(building->getPosition());
			rangeSprites->addChild(rangeSprite);
		}
	}
	else{
		rangeSprites->removeAllChildrenWithCleanup(true);
	}
}


void MapLayer::setTileMark(Point pos, bool canBuild){
	Point loc = this->mapCoordForPosition(pos, 1);
	tileMark->removeChildByName("mark");
	if (canBuild){
		Sprite* mark = Sprite::create("100_80_green.png");
		mark->setAnchorPoint(ccp(0, 0));
		mark->setPosition(ccp(100 * loc.x, 41 + 82 * loc.y));
		tileMark->addChild(mark, 0, "mark");
	}
	else{
		Sprite* mark = Sprite::create("100_80_red.png");
		mark->setAnchorPoint(ccp(0, 0));
		mark->setPosition(ccp(100 * loc.x, 41 + 82 * loc.y));
		tileMark->addChild(mark, 0, "mark");
	}
}

void MapLayer::removeTileMark(){
	tileMark->removeAllChildren();
}