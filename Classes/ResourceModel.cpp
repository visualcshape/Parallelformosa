#include "ResourceModel.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

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
	for (int i = 1; i <= TROOP_SIZE; i++){
		char buffer[30];
		sprintf(buffer, "%03d", i);
		std::string str("PlanetCute/Character");
		str += std::string(buffer);
		str += std::string(".png");
		CCLOG("%s\n", str.c_str());
		CCASSERT(CCFileUtils::getInstance()->isFileExist(str), "Troop Resource Loading Fail!!");
		strTroop[i] = str;
		GIDTroop[i] = 18 + i;
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

	//@var setup troop resource cost
	for (int i = 1; i <= TROOP_SIZE; i++){
		costLstr[i] = 100 * i;
		costGmag[i] = 55 * i;
		costFood[i] = i / 3 + 1;
	}
}

ResourceModel::~ResourceModel(){
}

ResourceModel* ResourceModel::getModel(){
	if (rm_pInstance == nullptr)
		rm_pInstance = new ResourceModel();
	return rm_pInstance;
}

void ResourceModel::CreateDownloadedDir(std::string relativePath){
	string pathToSave = CCFileUtils::getInstance()->getWritablePath();
	pathToSave += relativePath;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	DIR *pDir = NULL;
	pDir = opendir(pathToSave.c_str());
	if (!pDir)//The folder does not exist then began to create
	{
		mkdir(pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
#else
	if ((GetFileAttributesA(pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES) //The Win32 platform
	{
		CreateDirectoryA(pathToSave.c_str(), 0);
	}
#endif
}

bool ResourceModel::isFileExist(std::string pFileName){
	if (!pFileName.c_str()) return false;
	std::string filePath = CCFileUtils::getInstance()->getWritablePath();
	filePath += pFileName;
	FILE *pFp = fopen(filePath.c_str(), "r");
	CCLOG("%s", filePath.c_str());
	if (pFp)
	{
		fclose(pFp);
		return true;
	}
	return false;
}

void ResourceModel::copyData(std::string pFileName){
	std::string strPath = CCFileUtils::getInstance()->fullPathForFilename(pFileName);
	ssize_t len = 0;
	unsigned char* data = NULL;
	data = CCFileUtils::getInstance()->getFileData(strPath.c_str(), "r", &len);

	std::string destPath = CCFileUtils::getInstance()->getWritablePath();
	destPath += pFileName;

	FILE *pFp = fopen(destPath.c_str(), "w+");
	fwrite(data, sizeof(char), len, pFp);
	fclose(pFp);
	delete[]data;
	data = NULL;
}

bool ResourceModel::CreateDirectory(std::string pPath)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	mode_t processMask = umask(0); //Linux system setting file permissions
	int nRet = mkdir(pPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	umask(processMask);
	if (nRet != 0 && (errno != EEXIST))
	{
		return false;
	}
	return true;
#else
	BOOL nRet = CreateDirectoryA(pPath.c_str(), NULL);
	if (!nRet && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
	return true;
#endif
}

vector <string> ResourceModel::DecomposePath(std::string relativePath){
	int lenPtr = SZ(relativePath) - 1;
	vector <string> paths;
	while (lenPtr >= 0){
		while (lenPtr >= 0 && relativePath.at(lenPtr) != '/')
			lenPtr--;
		if (lenPtr > 0)
			paths.push_back(relativePath.substr(0, lenPtr));
		lenPtr--;
	}
	reverse(paths.begin(), paths.end());
	return paths;
}

FILE* ResourceModel::OpenFileR(string relativePath){
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	if (!isFileExist(relativePath)){
		vector <string> paths = DecomposePath(relativePath);
		for (int k = 0; k < SZ(paths); k++) if (!isFileExist(paths.at(k)))
			CreateDownloadedDir(paths.at(k));
		copyData(relativePath);
	}
	string strPath = CCFileUtils::getInstance()->getWritablePath() + relativePath;
	FILE* fp = fopen(strPath.c_str(), "r");
#else
	std::string fullPath = CCFileUtils::getInstance()->fullPathForFilename(relativePath);
	FILE *fp = fopen(fullPath.c_str(), "r");

	//@brief create empty file
	if (fp == nullptr){
		fp = fopen(fullPath.c_str(), "w");
		fclose(fp);
		fp = fopen(fullPath.c_str(), "r");
	}
#endif

	CCASSERT(fp != nullptr, "OpenFileR failed");
	return fp;
}

FILE* ResourceModel::OpenFileW(string relativePath){
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	vector <string> paths = DecomposePath(relativePath);
	for (int k = 0; k < SZ(paths); k++) if (!isFileExist(paths.at(k)))
		CreateDownloadedDir(paths.at(k));
	string strPath = CCFileUtils::getInstance()->getWritablePath() + relativePath;
	FILE* fp = fopen(strPath.c_str(), "w");
#else
	string fullPath = CCFileUtils::getInstance()->fullPathForFilename(relativePath);
	FILE *fp = fopen(fullPath.c_str(), "w");
#endif

	CCASSERT(fp != nullptr, "OpenFileW failed");
	return fp;
}
