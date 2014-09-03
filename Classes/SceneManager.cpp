#include "SceneManager.h"
#include "GameLayer\TrademarkLayer.h"
#include "GameLayer\LoginTitleLayer.h"
#include "GameLayer\MapLayer.h"
USING_NS_CC;

SceneManager::SceneManager(){
}

SceneManager::~SceneManager(){
}

void SceneManager::goTrademarkScreen(){
	Layer* layer = TrademarkLayer::create();
	SceneManager::go(layer);
}

void SceneManager::goTitleScreen(){
	Layer* layer = LoginTitleLayer::create();
	SceneManager::go(layer, 0.32f, Color3B::BLACK);
}

void SceneManager::goMapScreen(){
	Layer* layer = MapLayer::create();
	CCLOG("enter mappppppppp");
	SceneManager::go(layer, 0.32f, Color3B::BLACK);
}

void SceneManager::go(Layer* layer, float duration, Color3B cr){
	Director* director = Director::getInstance();
	Scene *newScene = SceneManager::wrap(layer);
	if (director->getRunningScene()){
		Director::getInstance()->replaceScene(TransitionFade::create(duration, newScene, cr));
	}
	else
		director->runWithScene(newScene);
}

Scene* SceneManager::wrap(Layer* layer){
	Scene* newScene = Scene::create();
	newScene->addChild(layer);
	return newScene;
}