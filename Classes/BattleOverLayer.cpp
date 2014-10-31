#include "BattleOverLayer.h"
#include "SceneManager.h"
#include "VisibleRect.h"
#include "AppMacro.h"

USING_NS_CC;

BattleOverLayer::BattleOverLayer(){
}

BattleOverLayer::~BattleOverLayer(){
}

BattleOverLayer* BattleOverLayer::create(std::string strLabel){
	BattleOverLayer *pBOL = new BattleOverLayer();
	if (pBOL && pBOL->init(strLabel)) {
		pBOL->autorelease();
		return pBOL;
	}
	CC_SAFE_DELETE(pBOL);
	return NULL;
}

bool BattleOverLayer::init(std::string strLabel){
	if (LayerColor::initWithColor(Color4B(255, 255, 255, 255))){
		auto winSize = Director::getInstance()->getWinSize();
		this->_label = LabelTTF::create(strLabel, "Artial", 32);
		_label->setColor(Color3B(0, 0, 0));
		_label->setPosition(Point(winSize.width / 2, winSize.height / 2));
		this->addChild(_label);

		this->runAction(Sequence::create(
			DelayTime::create(3),
			CallFunc::create(CC_CALLBACK_0(BattleOverLayer::battleOverDone, this)),
			NULL));

		return true;
	}
	return false;
}

void BattleOverLayer::battleOverDone(){
	ResourceModel *rm = ResourceModel::getModel();
	string mapName = MapModel::getModel()->getMapName();
	int firstMap, secondMap;
	sscanf(mapName.c_str(), "%d.%d", &firstMap, &secondMap);
	char buffer[1000];
	sprintf(buffer, "%d.%d.tmx", firstMap, secondMap);
	SceneManager::goMapScreen(string(buffer), HUD_ID::DEFENSE);
}
