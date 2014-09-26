#include "MapLayer.h"
#include "HUDLayer.h"
#include "MapModel.h"
#include "Building.h"
#include "SceneManager.h"
#include "AppMacro.h"
#include "ResourceModel.h"
#include "ResourceModel.h"
#include "DialogueWindowConfirm.h"
#include "VisibleRect.h"
#include "AppMacro.h"
#include <ctime>

USING_NS_CC;

MapModel* MapModel::mm_pInstance;

MapModel::MapModel(){
}

MapModel::~MapModel(){
}

MapModel* MapModel::getModel(){
	if (mm_pInstance == nullptr)
		mm_pInstance = new MapModel();
	return mm_pInstance;
}

void MapModel::init(std::string mapName){
	this->mapName = mapName;
	_buildings.clear();
	_pfLayers.clear(); 
	_movableSprites.clear();
	_tileMap = nullptr;
	_prevCursurOutside = false;
}

void MapModel::loadLayers(Vector <TMXLayer*> &tileLayers, std::string prefix){
	for (int lr = 0; true; lr++){
		char buffer[30];
		sprintf(buffer, "%d", lr);
		std::string str(prefix);
		str += " ";
		str += std::string(buffer);
		auto tmpLayer = _tileMap->layerNamed(str);
		if (!tmpLayer) break;
		tmpLayer->setAnchorPoint(Point(0, 0));
		tileLayers.pushBack(tmpLayer);
	}
}

Point MapModel::tileCoordForPosition(Point position){
	int x = position.x / _tileMap->getTileSize().width;
	int y = (_tileMap->getMapSize().height *_tileMap->getTileSize().height - position.y) / _tileMap->getTileSize().height;

	return Point(x, y);
}

Point MapModel::mapCoordForPosition(Point position, int level){
	Point TileLoc = this->tileCoordForPosition(position);
	const int MH = _tileMap->getMapSize().height;
	int row = MH - 2 - level - TileLoc.y;
	row = (row) >> 1;

	return Point(TileLoc.x, row);
}

void MapModel::addBuilding(Point pos, int level){
	if (!~canBuildOnTilePosition(pos)){
		CCLOG("Tile Not Buildable");
		return;
	}

	Point buildingLoc = this->mapCoordForPosition(pos, level);
	Point tileLoc = tileCoordForPosition(pos);
	Point tileBuildingLoc = Point(tileLoc.x, (int)(tileLoc.y + 1 - (level & 1)) / 2 * 2 + (level & 1));

	CCLOG(">> tileBuildingLoc %.0f %.0f", tileBuildingLoc.x, tileBuildingLoc.y);
	const int TW = _tileMap->getTileSize().width;
	const int TH = 50;

	Building* target = Building::build(selID);

	target->setAnchorPoint(Point(0, 0));
	target->setPosition(Point((buildingLoc.x * TW) + TW / 2, (buildingLoc.y * TH + level * 25 + 75)));
	target->setCoord(buildingLoc);
	target->height = level;

	for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++){
		_pfLayers.at(level + 1)->setTileGID(target->id, Point(tileBuildingLoc.x + tr, tileBuildingLoc.y - tc * 2));
	}

	_buildings.pushBack(target);
}

/*@return the highest layer could build, otherwise NONE
*/
int MapModel::canBuildOnTilePosition(Point pos){
	Point TileLoc = tileCoordForPosition(pos);
	
	//@debug modify label.
	{
		char buffer[30];
		sprintf(buffer, "row:%.0f, col:%.0f", TileLoc.x, TileLoc.y);
		getlblTilePos()->setString(buffer);
	}
	
	//@var later need to Resoucre Manager
	Building *target = Building::build(selID);

	for (int lr = SZ(_pfLayers) - 1; lr >= 0; lr--){
		Point buildingLoc = mapCoordForPosition(pos, lr);

		//@procedure check for no tiles on the tile.
		bool noTileOnDirectly = true;
		if (lr < SZ(_pfLayers) - 1){
			for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++){
				for (int offset = 0; offset <= 1; offset++){
					Point checkTileLoc = Point(TileLoc.x + tr, TileLoc.y - tc * 2 + offset);
					if (isTileInsideLayer(checkTileLoc, lr)){
						int tileGid = _pfLayers.at(lr + 1)->getTileGIDAt(checkTileLoc);
						if (tileGid != EMPTY_TILE) noTileOnDirectly = false;
					}
				}
			}
		}
		if (!noTileOnDirectly) continue;

		int couldTileCnt = 0;
		for (int tr = 0; tr < target->occupy.X; tr++) for (int tc = 0; tc < target->occupy.Y; tc++){
			for (int offset = 1; offset <= 2; offset++){
				Point checkTileLoc = Point(TileLoc.x + tr, TileLoc.y - tc * 2 + offset);
				if (isTileInsideLayer(checkTileLoc, lr)){
					int tileGid = _pfLayers.at(lr)->getTileGIDAt(checkTileLoc);

					Value props = _tileMap->getPropertiesForGID(tileGid);
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

bool MapModel::isTileInsideLayer(Point checkTileLoc, int level){
	if (checkTileLoc.x < 0) return false;
	if (checkTileLoc.x >= _pfLayers.at(level)->getLayerSize().width) return false;
	if (checkTileLoc.y < 0) return false;
	if (checkTileLoc.y >= _pfLayers.at(level)->getLayerSize().height) return false;
	return true;
}

Point MapModel::boundLayerPos(Point newPos){
	Size winSize = Director::getInstance()->getWinSize();
	Point retval = newPos;

	// inside the map, don't over border.
	retval.x = MIN(retval.x, 0);
	retval.x = MAX(retval.x, -_pfLayers.at(0)->getContentSize().width + winSize.width);

	retval.y = MIN(retval.y, 0);
	retval.y = MAX(retval.y, -_pfLayers.at(0)->getContentSize().height + winSize.height);

	return retval;
}

void MapModel::showAllRange(bool visible){
	_rangeSprites->removeAllChildren();
	if (visible){
		for (auto &building : _buildings){
			ResourceModel *rm = ResourceModel::getModel();
			auto rangeSprite = Sprite::create(rm->strRangePic);
			rangeSprite->setScale(4);
			rangeSprite->setPosition(building->getPosition());
			_rangeSprites->addChild(rangeSprite);
		}
	}
}

bool MapModel::tryTouchBegan(){
	Sprite * newSprite = NULL;
	Building* target = NULL;
	for (int i = 0; i < _movableSprites.size(); i++){
		Sprite* sprite = _movableSprites.at(i);
		Rect pos_rect = Rect((sprite->getPositionX() - sprite->getContentSize().width / 2),
			(sprite->getPositionY() - sprite->getContentSize().height / 2),
			sprite->getContentSize().width,
			sprite->getContentSize().height);
		float xMin = pos_rect.getMinX();
		float xMax = pos_rect.getMaxX();
		float yMIn = pos_rect.getMinY();
		float yMax = pos_rect.getMaxY();
		if (pos_rect.containsPoint(_touchLocation)){
			ResourceModel *rm = ResourceModel::getModel();
			_selSpriteRange = Sprite::create(rm->strRangePic);
			_selSpriteRange->setScale(4);
			_selGroups->setPosition(sprite->getPosition());
			_selGroups->addChild(_selSpriteRange);

			newSprite = Sprite::createWithTexture(sprite->getTexture()); //sprite;
			_selSprite = newSprite;
			_selGroups->addChild(newSprite);

			selID = i + 1;
			showAllRange(true);
		}
	}
	return true;
}

void MapModel::tryTouchMoved(){
	Point translation = _touchLocation - _oldTouchLocation;

	if (_prevCursurOutside){
		_selGroups->setPosition(_touchLocation);
		_prevCursurOutside = false;
	}

	if (_selSprite){
		_selGroups->setPosition(_selGroups->getPosition() + translation);


		//@debug modify label.
		char buffer[30];
		sprintf(buffer, "x:%.1f, y:%.1f", _touchLocationInGameLayer.x, _touchLocationInGameLayer.y);
		getlblCursorPos()->setString(buffer);

		int isBuildableLevel = canBuildOnTilePosition(_touchLocationInGameLayer);
		if (~isBuildableLevel)
			_selSprite->setOpacity(200);
		else
			_selSprite->setOpacity(50);
	}
	else{
		slide(translation);
	}
}

void MapModel::tryTouchEnded(){
	ResourceModel *rm = ResourceModel::getModel();

	//@brief just click once
	CCLOG("ccpdist. %.2f\n", _pressLoc.distance(_touchLocation));
	if (_pressLoc.distance(_touchLocation) < 6.5f){
		if (mapName.compare(rm->strWorldMap) == 0){
			Point tileLoc = tileCoordForPosition(_touchLocationInGameLayer);
			SceneManager::goMapScreen(rm->strTileMap[(int)tileLoc.x / 3]);
		}
		else{
			SceneManager::goMapScreen(rm->strWorldMap);
		}
		return;
	}

	if (_selSprite){
		Rect backgroundRect = Rect(_background->getPositionX(),
			_background->getPositionY(),
			_background->getContentSize().width,
			_background->getContentSize().height);

		if (!backgroundRect.containsPoint(_touchLocation)){
			int isBuildableLevel = canBuildOnTilePosition(_touchLocationInGameLayer);
			if (~isBuildableLevel)
				addBuilding(_touchLocationInGameLayer, isBuildableLevel);
		}

		_selGroups->removeAllChildren();
		_selSprite = NULL;
		_selSpriteRange = NULL;

		showAllRange(false);
	}
}

bool MapModel::outsideBordor(Size contentSize, Point pos){
	//@remind use BORDER_PIXEL pixel to avoid border error
	_prevCursurOutside = false;
	if (pos.x < BORDER_PIXEL || pos.x + BORDER_PIXEL > contentSize.width)
		_prevCursurOutside = true;
	if (pos.y < BORDER_PIXEL || pos.y + BORDER_PIXEL > contentSize.height)
		_prevCursurOutside = true;
	return _prevCursurOutside;
}

void MapModel::slide(Vec2 translation){
	Point oldMapBasePosition = _mapBasePosition;
	_mapBasePosition = boundLayerPos(_mapBasePosition + translation);
	Point HUDtranstion = _mapBasePosition - oldMapBasePosition;
	_hudBasePosition = _hudBasePosition - HUDtranstion;
	Notify();
}

void MapModel::refresh(float dt){
	if (_selSprite){
		if (_selGroups->getPositionX() < BORDER_PIXEL)
			slide(Vec2(SLIDE_RATE, 0));
		if (_selGroups->getPositionX() >= _mapContent.width - BORDER_PIXEL)
			slide(Vec2(-SLIDE_RATE, 0));
		if (_selGroups->getPositionY() < BORDER_PIXEL)
			slide(Vec2(0, SLIDE_RATE));
		if (_selGroups->getPositionY() >= _mapContent.height - BORDER_PIXEL)
			slide(Vec2(0, -SLIDE_RATE));
	}
}
