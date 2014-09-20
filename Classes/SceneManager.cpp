#include "SceneManager.h"
#include "GameLayer/TrademarkLayer.h"
#include "GameLayer/LoginTitleLayer.h"
#include "GameLayer/MapLayer.h"
#include "Model/ResourceModel.h"
#include "Model/DataModel.h"
USING_NS_CC;

SceneManager::SceneManager(){
}

SceneManager::~SceneManager(){
}

void SceneManager::cleanRunningScreen(){
	if (Director::getInstance()->getRunningScene())
		Director::getInstance()->getRunningScene()->removeAllChildren();
}

void SceneManager::goTrademarkScreen(){
	SceneManager::cleanRunningScreen();
	Layer* layer = TrademarkLayer::create();
	SceneManager::go(layer);
}

void SceneManager::goTitleScreen(){
	SceneManager::cleanRunningScreen();
	Layer* layer = LoginTitleLayer::create();
	SceneManager::go(layer, 0.32f, Color3B::BLACK);
}

void SceneManager::goMapScreen(std::string mapName){
	DataModel *dm = DataModel::getModel();
	dm->setMapName(mapName);

	SceneManager::cleanRunningScreen();
	Layer* layer = MapLayer::create();
	SceneManager::go(layer, 0.32f, Color3B::BLACK);
}

void SceneManager::go(Layer* layer, float duration, Color3B cr){
	Director* director = Director::getInstance();
	Scene *newScene = SceneManager::wrap(layer);
	if (director->getRunningScene()){
		director->replaceScene(TransitionFade::create(duration, newScene, cr));
	}
	else
		director->runWithScene(newScene);
}

Scene* SceneManager::wrap(Layer* layer){
	Scene* newScene = Scene::create();
	newScene->addChild(layer);
	return newScene;
}

void SceneManager::pressKeyCode(EventKeyboard::KeyCode keyCode){
	DataModel *dm = DataModel::getModel();
	ResourceModel *rm = ResourceModel::getModel();

	if (keyCode == EventKeyboard::KeyCode::KEY_R){
		CCLog("R key was pressed");
		SceneManager::goTitleScreen();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_W){
		CCLog("W key was pressed");
		SceneManager::goTrademarkScreen();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_M){
		CCLog("M key was pressed");
		if (dm->getMapName().compare(rm->strWorldMap) != 0)
			SceneManager::goMapScreen(rm->strWorldMap);
	}
}
