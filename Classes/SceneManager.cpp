#include "SceneManager.h"
#include "TrademarkLayer.h"
#include "LoginTitleLayer.h"
#include "BattleOverLayer.h"
#include "MapLayer.h"
#include "HUDLayer.h"
#include "ResourceModel.h"
#include "MainScene.h"
#include "CommandModel.h"

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

void SceneManager::goMapScreen(std::string mapName, HUD_ID status){
	SceneManager::cleanRunningScreen();
	Layer* layer = MapLayer::create(mapName);

	//@brief bind mainUIlayer
	auto mainMenuUILayer = MainMenuLayer::create();
	layer->addChild(mainMenuUILayer, 10000);
	auto mainMenuInfoLayer = MainInfoLayer::create();
	layer->addChild(mainMenuInfoLayer, 10001);

	
	//@brief bind HUDlayer
	auto hudlayer = HUDLayer::create(status);
	layer->addChild(hudlayer, 20000);

	SceneManager::go(layer, 0.32f, Color3B::BLACK);
}

void SceneManager::goBattleOverScreen(std::string strLabel){
	SceneManager::cleanRunningScreen();
	Layer* layer = BattleOverLayer::create(strLabel);
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
	layer->schedule(schedule_selector(CommandModel::execute), EXECUTE_RATE);
	return newScene;
}

void SceneManager::pressKeyCode(EventKeyboard::KeyCode keyCode){
	ResourceModel *rm = ResourceModel::getModel();

	if (keyCode == EventKeyboard::KeyCode::KEY_R){
		CCLOG("R key was pressed");
		SceneManager::goTitleScreen();
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_W){
		CCLOG("W key was pressed");
		SceneManager::goTrademarkScreen();
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_M){
		CCLOG("M key was pressed");
		SceneManager::goMapScreen(rm->strWorldMap, HUD_ID::DEFENSE);
	}
}
