#include "GameLayer\MapLayer.h"
#include "GameLayer\HUDLayer.h"
#include "Model\DataModel.h"

USING_NS_CC;

MapLayer::MapLayer(){
}

MapLayer::~MapLayer(){
}

bool MapLayer::init(){
	if (!Layer::init())
		return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();

	auto myHUDLayer = HUDLayer::shareHUD();

	myHUDLayer->setPosition(Vec2(visibleOrigin.x, visibleOrigin.y));
	// add layer as a child to scene
	this->addChild(myHUDLayer, 100);

	DataModel* m = DataModel::getModel();
	m->setGameLayer(this);
	m->setMyHUDLayer(myHUDLayer);

	this->tileMap = TMXTiledMap::create("TileMap.tmx");
	this->background = tileMap->layerNamed("Background");
	this->background->setAnchorPoint(ccp(0, 0));
	this->addChild(tileMap, 0);

	this->scheduleUpdate();

	return true;
}

Point MapLayer::tileCoordForPosition(Point position){
	int x = position.x / this->tileMap->getTileSize().width;
	int y = (this->tileMap->getMapSize().height *this->tileMap->getTileSize().height - position.y) / this->tileMap->getTileSize().height;

	return ccp(x, y);
}

void MapLayer::addTower(Point pos){
	DataModel *m = DataModel::getModel();
	Tower *target = NULL;
	Point towerLoc = this->tileCoordForPosition(pos);

	int tileGid = this->background->tileGIDAt(towerLoc);
	Value props = this->tileMap->propertiesForGID(tileGid);
	bool buildable = !props.isNull();
	if (!props.isNull()){
		ValueMap map = props.asValueMap();

		int type_int = map.at("buildable").asInt();
		if (1 == type_int){
			target = MachineGunTower::tower();
			target->setPosition(ccp((towerLoc.x * 32) + 16, this->tileMap->getContentSize().height - (towerLoc.y * 32) - 16));
			target->setCoord(towerLoc);
			this->addChild(target, 1);
			target->setTag(1);
			m->getTowers().pushBack(target);
		}
		else
			buildable = false;
	}
	if (!buildable) CCLOG("Tile Not Buildable");
}

bool MapLayer::canBuildOnTilePosition(Point pos){
	Point towerLoc = this->tileCoordForPosition(pos);
	int tileGid = this->background->getTileGIDAt(towerLoc);
	DataModel* m = DataModel::getModel();

	for each(Tower *tower in m->getTowers()){
		if (tower->getCoord() == towerLoc)
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

	return false;
}

Point MapLayer::boundLayerPos(Point newPos){
	Size winSize = Director::getInstance()->getWinSize();
	Point retval = newPos;

	// inside the map, don't over border.
	retval.x = MIN(retval.x, 0);
	retval.x = MAX(retval.x, -background->getContentSize().width + winSize.width);

	retval.y = MIN(retval.y, 0);
	retval.y = MAX(retval.y, -background->getContentSize().height + winSize.height);

	return retval;
}

void MapLayer::panForTranslation(Point translation){
	Point newPos = this->getPosition() + translation;
	this->setPosition(this->boundLayerPos(newPos));
}
