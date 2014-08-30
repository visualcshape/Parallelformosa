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
	this->addChild(myHUDLayer, 1);

	DataModel* m = DataModel::getModel();
	m->_gameLayer = this;
	m->_HUDLayerLayer = myHUDLayer;

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
			m->towers.pushBack(target);
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

	for each(Tower *tower in m->towers){
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
	Size winSize = CCDirector::getInstance()->getWinSize();
	Point retval = newPos;
	retval.x = MIN(retval.x, 0);
	retval.x = MAX(retval.x, tileMap->getContentSize().width + winSize.width);
	retval.y = MIN(0, retval.y);
	retval.y = MAX(tileMap->getContentSize().height + winSize.height, retval.y);
	return retval;
}