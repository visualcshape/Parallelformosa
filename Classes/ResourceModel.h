#pragma once
#include "cocos2d.h"

USING_NS_CC;

class ResourceModel{
	static const int BUILDING_SIZE = 5;
	static const int CHARACTER_SIZE = 5;
	static const int TERRAIN_SIZE = 11;
	static const int TILEMAP_SIZE = 9;
	static const int PALYER_SIZE = 2;

public:
	ResourceModel();
	~ResourceModel();

	static ResourceModel* getModel();

	std::string strBuilding[BUILDING_SIZE + 1];
	std::string strCharacter[CHARACTER_SIZE + 1];
	std::string strTerrain[TERRAIN_SIZE + 1];

	std::string strWorldMap;
	std::string strTileMap[TILEMAP_SIZE + 1];
	std::string strPlayerMap[PALYER_SIZE + 1];

	std::string strRangePic;
	std::string strHUDPic;

	int GIDBuilding[BUILDING_SIZE + 1];
	int GIDCharacter[CHARACTER_SIZE + 1];
	int GIDTerrain[TERRAIN_SIZE + 1];
protected:
	static ResourceModel* rm_pInstance;
};
