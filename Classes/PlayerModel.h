#pragma once
#include "cocos2d.h"
#include "Building.h"

USING_NS_CC;

class PlayerModel{

public:
	PlayerModel();
	~PlayerModel();

	static PlayerModel* getModel();

	int height;
	Point coord;

protected:
	static PlayerModel* pm_pInstance;
};
