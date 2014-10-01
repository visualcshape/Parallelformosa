#include "ResourceModel.h"

USING_NS_CC;

ResourceModel* ResourceModel::rm_pInstance;

ResourceModel::ResourceModel(){
	//@var load building
	for (int i = 1; i <= BUILDING_SIZE; i++){
		char buffer[30];
		sprintf(buffer, "%03d", i);
		std::string str("PlanetCute/Building");
		str += std::string(buffer);
		str += std::string(".png");
		CCLOG(">> Load %s\n", str.c_str());
		CCASSERT(CCFileUtils::getInstance()->isFileExist(str), "Building Resource Loading Fail!!");
		strBuilding[i] = str;
		GIDBuilding[i] = 36 + i;
	}

	//@var load character
	for (int i = 1; i <= CHARACTER_SIZE; i++){
		char buffer[30];
		sprintf(buffer, "%03d", i);
		std::string str("PlanetCute/Character");
		str += std::string(buffer);
		str += std::string(".png");
		CCLOG("%s\n", str.c_str());
		CCASSERT(CCFileUtils::getInstance()->isFileExist(str), "Character Resource Loading Fail!!");
		strCharacter[i] = str;
		GIDCharacter[i] = 18 + i;
	}

	//@var load terrain
	for (int i = 1; i <= TERRAIN_SIZE; i++){
		char buffer[30];
		sprintf(buffer, "%03d", i);
		std::string str("PlanetCute/Terrain");
		str += std::string(buffer);
		str += std::string(".png");
		CCLOG("%s\n", str.c_str());
		CCASSERT(CCFileUtils::getInstance()->isFileExist(str), "Terrain Resource Loading Fail!!");
		strTerrain[i] = str;
		GIDTerrain[i] = i;
	}

	//@var load map
	strWorldMap = std::string("PFMAP/pfmap_world.tmx");
	CCASSERT(CCFileUtils::getInstance()->isFileExist(strWorldMap), "Map Resource Loading Fail!!");
	for (int i = 0; i < TILEMAP_SIZE; i++){
		char buffer[30];
		sprintf(buffer, "%03d", i);
		std::string str("PFMAP/pfmap");
		str += std::string(buffer);
		str += std::string(".tmx");
		CCASSERT(CCFileUtils::getInstance()->isFileExist(str), "Map Resource Loading Fail!!");
		strTileMap[i] = str;
	}

	//@var load map
	for (int i = 1; i <= PALYER_SIZE; i++){
		char buffer[30];
		sprintf(buffer, "%d", i);
		std::string str("PFMAP/PlayerBase/player");
		str += std::string(buffer);
		str += std::string(".tmx");
		CCASSERT(CCFileUtils::getInstance()->isFileExist(str), "Player Map Resource Loading Fail!!");
		strPlayerMap[i] = str;
	}

	//@var load range pic
	strRangePic = std::string("Range.png");
	CCASSERT(CCFileUtils::getInstance()->isFileExist(strRangePic), "RangePic Resource Loading Fail!!");

	//@var load hud bar pic
	strHUDPic = std::string("hud.png");
	CCASSERT(CCFileUtils::getInstance()->isFileExist(strHUDPic), "HUDBARPic Resource Loading Fail!!");

}

ResourceModel::~ResourceModel(){
}

ResourceModel* ResourceModel::getModel(){
	if (rm_pInstance == nullptr)
		rm_pInstance = new ResourceModel();
	return rm_pInstance;
}