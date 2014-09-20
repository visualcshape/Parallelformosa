#pragma once
#include "cocos2d.h"

USING_NS_CC;

class SceneManager :public Ref{
public:
	SceneManager();
	~SceneManager();

	static void goTrademarkScreen();
	static void goTitleScreen();
	static void goMapScreen(std::string mapName);
	static void pressKeyCode(EventKeyboard::KeyCode keyCode);

protected:
	static void cleanRunningScreen();

private:
	static void go(Layer *layer, float duration = 0.0f, Color3B cr = Color3B::BLACK);
	static Scene* wrap(Layer *layer);
};