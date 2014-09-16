#pragma once
#include "cocos2d.h"

USING_NS_CC;

class ResourceModel{
	static const int BUILDING_SIZE = 5;
	static const int CHARACTER_SIZE = 6;
	static const int TERRAIN_SIZE = 11;

public:
	ResourceModel();
	~ResourceModel();

	static ResourceModel* getModel();
	Sprite* picBuilding[BUILDING_SIZE + 1];
	Sprite* picCharacter[CHARACTER_SIZE + 1];
	Sprite* picTerrain[TERRAIN_SIZE + 1];

	int GIDBuilding[BUILDING_SIZE + 1];
	int GIDCharacter[CHARACTER_SIZE + 1];
	int GIDTerrain[TERRAIN_SIZE + 1];

protected:
	static ResourceModel* rm_pInstance;
};
