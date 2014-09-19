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
		CCLOG("%s\n", str.c_str());
		CCASSERT(CCFileUtils::getInstance()->isFileExist(str), "Building Resource Loading Fail!!");
		picBuilding[i] = Sprite::create(str);
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
		picCharacter[i] = Sprite::create(str);
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
		picTerrain[i] = Sprite::create(str);
		GIDTerrain[i] = i;
	}
}

ResourceModel::~ResourceModel(){
}

ResourceModel* ResourceModel::getModel(){
	if (rm_pInstance == NULL)
		rm_pInstance = new ResourceModel();
	return rm_pInstance;
}