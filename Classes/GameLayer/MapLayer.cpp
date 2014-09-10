#include "GameLayer\MapLayer.h"
#include "GameLayer\HUDLayer.h"
#include "Model\DataModel.h"
#include "Map\Building.h"
#include "SceneManager.h"
#include "AppMacro.h"

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
	this->addChild(myHUDLayer, 10000);

	rangeSprites = Node::create();
	this->addChild(rangeSprites, 5);

	tileMark = Node::create();
	this->addChild(tileMark, 5);

	DataModel* m = DataModel::getModel();
	m->setGameLayer(this);

	this->tileMap = TMXTiledMap::create("TestMap.tmx");
	for (int lr = 0; true; lr++){
		char buffer[30];
		sprintf(buffer, "%d", lr);
		std::string str("Tile Layer ");
		str += std::string(buffer);
		auto tileLayer = tileMap->layerNamed(str);
		if (!tileLayer) break;
		tileLayer->setAnchorPoint(ccp(0, 0));
		this->tileLayers.pushBack(tileLayer);
		CCLOG(">> at %d layer, zorder = %d, zcoord is %.2f\n", lr, tileLayer->getZOrder(), tileLayer->getVertexZ());
		for (int r = 0; r < 20; r++, puts("")) for (int c = 0; c < 12; c++){
			CCSprite *tile0 = tileLayer->tileAt(ccp(c, r));
			if (tile0) CCLOG(">>>> at (%d,%d) => %d ", c, r, tile0->getLocalZOrder());
		}
	}
	CCLOG(">> %d\n", SZ(tileLayers));
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
	const int MH =this->tileMap->getMapSize().height;
	int row = MH - 2 - level - TileLoc.y;
	row = (row) >> 1;

	return ccp(TileLoc.x, row);
}

void MapLayer::addBuilding(Point pos, int selID, int level){
	DataModel *m = DataModel::getModel();

	if (!~this->canBuildOnTilePosition(pos, selID)){
		CCLOG("Tile Not Buildable");
		return;
	}

	Point buildingLoc = this->mapCoordForPosition(pos, level);
	Point tileLoc = tileCoordForPosition(pos);
	Point tileBuildingLoc = ccp(tileLoc.x, (int)(tileLoc.y + 1 - (level & 1)) / 2 * 2 + (level & 1));

	CCLOG(">> tileBuildingLoc %.0f %.0f", tileBuildingLoc.x, tileBuildingLoc.y);
	const int TW = this->tileMap->getTileSize().width;
	const int TH = 82;

	Building* target = NULL;
	switch (selID){
	case 16: target = OrangeGem::build(); break;
	case 14: target = BlueGem::build(); break;
	case 15: target = GreenGem::build(); break;
	case 35: target = Star::build(); break;
	}

	target->setAnchorPoint(ccp(0, 0));
	target->setPosition(ccp((buildingLoc.x * TW) + TW / 2, (buildingLoc.y * TH + level * 41 + 123)));
	target->setCoord(buildingLoc);
	target->height = level;

	for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++){
		tileLayers.at(level + 1)->setTileGID(target->id, ccp(tileBuildingLoc.x + tr, tileBuildingLoc.y - tc * 2));
	}
	
	Vector <Building*> V = m->getBuildings(); V.pushBack(target);	m->setBuildings(V);
	CCLOG(">> ADD to layer %d, zorder=%d z=%d buildingLoc %.0f %.0f", level, target->getZOrder(), target->getLocalZOrder(), buildingLoc.x, buildingLoc.y);
}

/*@return the highest layer could build, otherwise NONE
 */
int MapLayer::canBuildOnTilePosition(Point pos, int selID){
	Point TileLoc = this->tileCoordForPosition(pos);

	//@debug modify label.
	{
		DataModel *m = DataModel::getModel();
		char buffer[30];
		sprintf(buffer, "row:%.0f, col:%.0f", TileLoc.x, TileLoc.y);
		m->getMyHUDLayer()->getlblTilePos()->setString(buffer);
	}

	//@var later need to Resoucre Manager
	Building *target = NULL;
	switch (selID){
	case 16: target = OrangeGem::build(); break;
	case 14: target = BlueGem::build(); break;
	case 15: target = GreenGem::build(); break;
	case 35: target = Star::build(); break;
	}

	//@debug modify label.
	{
		DataModel *m = DataModel::getModel();
		char buffer[30];
		sprintf(buffer, "bulding occpuy=%d,%d", target->occupy.X, target->occupy.Y);
		m->getMyHUDLayer()->getlblBuldingPos()->setString(buffer);
	}

	DataModel* m = DataModel::getModel();

	for (int lr = SZ(tileLayers) - 1; lr >= 0; lr--){
		CCLOG(">> at tile layer = %d", lr);
		Point buildingLoc = this->mapCoordForPosition(pos, lr);
		if (lr < SZ(tileLayers) - 1){
			for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++){
				for (int offset = 0; offset <= 1; offset++){
					Point checkTileLoc = ccp(TileLoc.x + tr, TileLoc.y - tc * 2 + offset);
					if (checkTileLoc.x < 0) continue;
					if (checkTileLoc.x >= this->tileLayers.at(lr)->getLayerSize().width) continue;
					if (checkTileLoc.y < 0) continue;
					if (checkTileLoc.y >= this->tileLayers.at(lr)->getLayerSize().height) continue;

					int tileGid = this->tileLayers.at(lr + 1)->getTileGIDAt(checkTileLoc);
					if (tileGid != 46) return -1;
				}
			}
		}

		int couldTileCnt = 0;
		for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++){
			for (int offset = 1; offset <= 2; offset++){
				Point checkTileLoc = ccp(TileLoc.x + tr, TileLoc.y - tc * 2 + offset);
				if (checkTileLoc.x < 0) continue; 
				if (checkTileLoc.x >= this->tileLayers.at(lr)->getLayerSize().width) continue;
				if (checkTileLoc.y < 0) continue;
				if (checkTileLoc.y >= this->tileLayers.at(lr)->getLayerSize().height) continue;

				int tileGid = this->tileLayers.at(lr)->getTileGIDAt(checkTileLoc);

				Value props = this->tileMap->getPropertiesForGID(tileGid);
				if (!props.isNull()){
					ValueMap map = props.asValueMap();
					int type_int = 0;
					if (map.size() == 0)
						type_int = 0;
					else
						type_int = map.at("buildable").asInt();
					if (1 == type_int){
						couldTileCnt++;
						break;
					}
				}
			}
		}
		if (couldTileCnt == target->occupy.X*target->occupy.Y) return lr;
	}
	return -1;
}

Point MapLayer::boundLayerPos(Point newPos){
	Size winSize = Director::getInstance()->getWinSize();
	Point retval = newPos;

	// inside the map, don't over border.
	retval.x = MIN(retval.x, 0);
	retval.x = MAX(retval.x, -tileLayers.at(0)->getContentSize().width + winSize.width);

	retval.y = MIN(retval.y, 0);
	retval.y = MAX(retval.y, -tileLayers.at(0)->getContentSize().height + winSize.height);

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

void MapLayer::setTileMark(Point pos, int selID, int level, bool canBuild){
	Point loc = this->mapCoordForPosition(pos, level);
	tileMark->removeAllChildren();

	Building *target = NULL;
	switch (selID){
	case 16: target = OrangeGem::build(); break;
	case 14: target = BlueGem::build(); break;
	case 15: target = GreenGem::build(); break;
	case 35: target = Star::build(); break;
	}

	for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++){
		if (canBuild){
			Sprite* mark = Sprite::create("100_80_green.png");
			mark->setAnchorPoint(ccp(0, 0));
			mark->setPosition(ccp(100 * (loc.x + tr), 41 * (level + 1) + 82 * (loc.y + tc)));
			tileMark->addChild(mark, 0);
		}
		else{
			Sprite* mark = Sprite::create("100_80_red.png");
			mark->setAnchorPoint(ccp(0, 0));
			mark->setPosition(ccp(100 * (loc.x + tr), 41 * (level + 1) + 82 * (loc.y + tc)));
			tileMark->addChild(mark, 0);
		}
	}
}
void MapLayer::removeTileMark(){
	tileMark->removeAllChildren();
}