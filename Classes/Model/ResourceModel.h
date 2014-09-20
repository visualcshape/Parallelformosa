#pragma once
#include "cocos2d.h"

USING_NS_CC;

class ResourceModel{
	static const int BUILDING_SIZE = 5;
	static const int CHARACTER_SIZE = 6;
	static const int TERRAIN_SIZE = 11;
	static const int TILEMAP_SIZE = 9;
public:
	ResourceModel();
	~ResourceModel();

	static ResourceModel* getModel();

	std::string strBuilding[BUILDING_SIZE + 1];
	std::string strCharacter[CHARACTER_SIZE + 1];
	std::string strTerrain[TERRAIN_SIZE + 1];

	std::string strWorldMap;
	std::string strTileMap[TILEMAP_SIZE + 1];

	int GIDBuilding[BUILDING_SIZE + 1];
	int GIDCharacter[CHARACTER_SIZE + 1];
	int GIDTerrain[TERRAIN_SIZE + 1];
protected:
	static ResourceModel* rm_pInstance;
};
