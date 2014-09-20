<<<<<<< HEAD:Classes/GameLayer/MapLayer.cpp
#include "GameLayer\MapLayer.h"
#include "GameLayer\HUDLayer.h"
#include "Model\DataModel.h"
#include "Model\MapModel.h"
#include "Map\Building.h"
=======
#include "MapLayer.h"
#include "HUDLayer.h"
#include "DataModel.h"
#include "MapModel.h"
#include "Building.h"
>>>>>>> v0.1.2_iOS:Classes/MapLayer.cpp
#include "SceneManager.h"
#include "AppMacro.h"
#include <ctime>

USING_NS_CC;

MapLayer::MapLayer(){
	CCLOG("Map Layer construct");
}

MapLayer::~MapLayer(){
	DataModel* m = DataModel::getModel();
	m->setGameLayer(NULL);
	CCLOG("Map Layer destruct");
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

	DataModel* dm = DataModel::getModel();
	dm->setGameLayer(this);

	double tstart = clock();
	this->tileMap = TMXTiledMap::create(dm->getMapName());
	
	loadLayers(&PFLayers, "PF Layer");
	loadLayers(&markLayers, "Mark Layer");

	CCLOG(">> %d\n", SZ(PFLayers));
	this->addChild(tileMap, 0);

	double tend = clock();
	double tcost = (double)(tend - tstart) / CLOCKS_PER_SEC;
	CCLOG(">>>>>> TIME NEED %lf\n", tcost);
	this->scheduleUpdate();

	//@debug conveniently use keyboard to restart in Win32.
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(MapLayer::keyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);


	//@debug modify label.
	{
		DataModel *m = DataModel::getModel();
		char buffer[70];
		sprintf(buffer, "map id = %s", m->getMapName().c_str());
		m->getMyHUDLayer()->getlblBuldingPos()->setString(buffer);
	}

	return true;
}

void MapLayer::loadLayers(Vector <TMXLayer*> *tileLayers, std::string prefix){
	for (int lr = 0; true; lr++){
		char buffer[30];
		sprintf(buffer, "%d", lr);
		std::string str(prefix);
		str += " ";
		str += std::string(buffer);
		auto tmpLayer = tileMap->layerNamed(str);
		if (!tmpLayer) break;
		tmpLayer->setAnchorPoint(ccp(0, 0));
		tileLayers->pushBack(tmpLayer);
	}
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
	const int TH = 50;

	Building* target = Building::build(selID);

	target->setAnchorPoint(ccp(0, 0));
	target->setPosition(ccp((buildingLoc.x * TW) + TW / 2, (buildingLoc.y * TH + level * 25 + 75)));
	target->setCoord(buildingLoc);
	target->height = level;

	for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++){
		PFLayers.at(level + 1)->setTileGID(target->id, ccp(tileBuildingLoc.x + tr, tileBuildingLoc.y - tc * 2));
	}
	
	Vector <Building*> V = m->getBuildings(); V.pushBack(target); m->setBuildings(V);
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
	Building *target = Building::build(selID);

	DataModel* m = DataModel::getModel();

	for (int lr = SZ(PFLayers) - 1; lr >= 0; lr--){
		Point buildingLoc = mapCoordForPosition(pos, lr);

		//@procedure check for no tiles on the tile.
		bool noTileOnDirectly = true;
		if (lr < SZ(PFLayers) - 1){
			for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++){
				for (int offset = 0; offset <= 1; offset++){
					Point checkTileLoc = ccp(TileLoc.x + tr, TileLoc.y - tc * 2 + offset);
					if (isTileInsideLayer(checkTileLoc, lr)){
						int tileGid = PFLayers.at(lr + 1)->getTileGIDAt(checkTileLoc);
						if (tileGid != EMPTY_TILE) noTileOnDirectly = false;
					}
				}
			}
		}
		if (!noTileOnDirectly) continue;

		int couldTileCnt = 0;
		for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++){
			for (int offset = 1; offset <= 2; offset++){
				Point checkTileLoc = ccp(TileLoc.x + tr, TileLoc.y - tc * 2 + offset);
				if (isTileInsideLayer(checkTileLoc, lr)){
					int tileGid = PFLayers.at(lr)->getTileGIDAt(checkTileLoc);

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
		}
		if (couldTileCnt == target->occupy.X*target->occupy.Y) return lr;
	}
	return -1;
}

bool MapLayer::isTileInsideLayer(Point checkTileLoc, int level){
	if (checkTileLoc.x < 0) return false;
	if (checkTileLoc.x >= PFLayers.at(level)->getLayerSize().width) return false;
	if (checkTileLoc.y < 0) return false;
	if (checkTileLoc.y >= PFLayers.at(level)->getLayerSize().height) return false;
	return true;
}

Point MapLayer::boundLayerPos(Point newPos){
	Size winSize = Director::getInstance()->getWinSize();
	Point retval = newPos;

	// inside the map, don't over border.
	retval.x = MIN(retval.x, 0);
	retval.x = MAX(retval.x, -PFLayers.at(0)->getContentSize().width + winSize.width);

	retval.y = MIN(retval.y, 0);
	retval.y = MAX(retval.y, -PFLayers.at(0)->getContentSize().height + winSize.height);

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
		for (auto &building : m->getBuildings()){
			auto rangeSprite = Sprite::create("Range.png");
			rangeSprite->setScale(4);
			rangeSprite->setPosition(building->getPosition());
			rangeSprites->addChild(rangeSprite);
		}
	}
	else{
		rangeSprites->removeAllChildren();
	}
}

void MapLayer::setTileMark(Point pos, int selID, int level, bool canBuild){
	Point tileLoc = tileCoordForPosition(pos);
	Point tileBuildingLoc = ccp(tileLoc.x, (int)(tileLoc.y + 1 - (level & 1)) / 2 * 2 + (level & 1));

	removeTileMark();

	Building *target = Building::build(selID);

	for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++){
		Point targetBuildingLoc = ccp(tileBuildingLoc.x + tr, tileBuildingLoc.y - tc * 2);
		if (isTileInsideLayer(targetBuildingLoc, level)){
			if (canBuild){
				markLayers.at(level)->setTileGID(GREEN_MARK, targetBuildingLoc);
				tileMark.push_back(Vec3(targetBuildingLoc.x, targetBuildingLoc.y, level));
			}
			/*@discard
			markLayers.at(level)->setTileGID(RED_MARK, targetBuildingLoc);
			Sprite* mark = Sprite::create("100_80_red.png");
			mark->setAnchorPoint(ccp(0, 0));
			mark->setPosition(ccp(100 * (loc.x + tr), 41 * (level + 1) + 82 * (loc.y + tc)));
			tileMark->addChild(mark, 0);*/
		}
	}
}

void MapLayer::removeTileMark(){
	for (auto mark : tileMark)
		markLayers.at(mark.z)->setTileGID(EMPTY_MARK, ccp(mark.x, mark.y));
	tileMark.clear();
}
