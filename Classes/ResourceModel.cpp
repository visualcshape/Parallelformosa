#include "ResourceModel.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

USING_NS_CC;

ResourceModel* ResourceModel::rm_pInstance;

ResourceModel::ResourceModel(){
}

ResourceModel::~ResourceModel(){
}

ResourceModel* ResourceModel::getModel(){
	if (rm_pInstance == nullptr)
		rm_pInstance = new ResourceModel();
	return rm_pInstance;
}

string ResourceModel::getSystemTimeString(){
	struct tm *tm;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time_t timep;
	time(&timep);
	tm = localtime(&timep);
//#else
	//struct cc_timeval now;
	//CCTime::gettimeofdayCocos2d(&now, NULL);
	//tm = localtime(&now.tv_sec);
//#endif
	int year = tm->tm_year + 1900;
	int month = tm->tm_mon + 1;
	int day = tm->tm_mday;
	int hour = tm->tm_hour;
	int min = tm->tm_min;
	int sec = tm->tm_sec;

	char buffer[1000];
	sprintf(buffer, "%04d-%02d-%02d_%02d:%02d:%02d", year, month, day, hour, min, sec);

	return string(buffer);
}

tm ResourceModel::makeSystemTime(int year, int month, int day, int hour, int min, int sec){
	tm tmp;
	tmp.tm_year = year - 1900;
	tmp.tm_mon = month - 1;
	tmp.tm_mday = day;
	tmp.tm_hour = hour;
	tmp.tm_min = min;
	tmp.tm_sec = sec;
	return tmp;
}

void ResourceModel::LoadBuildings(){
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
}

void ResourceModel::LoadTroops(){
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
}

void ResourceModel::LoadTerrains(){
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
}
/*
void ResourceModel::LoadTilemaps(){
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
}
void ResourceModel::LoadPlayers(){
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
}*/

void ResourceModel::LoadMISC(){
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

void ResourceModel::CreateDownloadedDir(std::string relativePath){
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	string pathToSave = CCFileUtils::getInstance()->getWritablePath();
	pathToSave += relativePath;
	DIR *pDir = NULL;
	pDir = opendir(pathToSave.c_str());
	if (!pDir)//The folder does not exist then began to create
	{
		mkdir(pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
#else
	string pathToSave = CCFileUtils::getInstance()->fullPathForFilename(relativePath);
	if ((GetFileAttributesA(pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES) //The Win32 platform
	{
		CreateDirectoryA(pathToSave.c_str(), 0);
	}
#endif
}

bool ResourceModel::isFileExist(std::string pFileName){
	if (!pFileName.c_str()) return false;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	std::string filePath = CCFileUtils::getInstance()->getWritablePath();
	filePath += pFileName;
#else
	std::string filePath = CCFileUtils::getInstance()->fullPathForFilename(pFileName);
#endif
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
	destPath += NoFolderPath(pFileName);

	FILE *pFp = fopen(destPath.c_str(), "w+");
	fwrite(data, sizeof(char), len, pFp);
	fclose(pFp);
	delete[]data;
	data = NULL;
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
string ResourceModel::NoFolderPath(string relativePath){
	int lenPtr = SZ(relativePath) - 1;
	char buffer[1000];
	int ptr = 0;
	for (int i = lenPtr; i >= 0 && relativePath.at(i) != '/'; i--)
		buffer[ptr++] = relativePath.at(i);
	reverse(buffer, buffer + ptr);
	buffer[ptr] = '\0';
	return string(buffer);
}

FILE* ResourceModel::OpenFileR(string relativePath){
	if (!isFileExist(relativePath)){
		vector <string> paths = DecomposePath(relativePath);
		for (int k = 0; k < SZ(paths); k++) if (!isFileExist(paths.at(k)))
			CreateDownloadedDir(paths.at(k));
	}

	string strPath = CCFileUtils::getInstance()->getWritablePath() + relativePath;
	FILE* fp = fopen(strPath.c_str(), "r");
//	std::string fullPath = CCFileUtils::getInstance()->fullPathForFilename(relativePath);
//	FILE *fp = fopen(fullPath.c_str(), "r");

	//@brief create empty file
	if (fp == nullptr){
		fp = fopen(strPath.c_str(), "w");
		fclose(fp);
		fp = fopen(strPath.c_str(), "r");
	}

	CCASSERT(fp != nullptr, "OpenFileR failed");
	return fp;
}

FILE* ResourceModel::OpenFileW(string relativePath){

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	relativePath = NoFolderPath(relativePath);
	string strPath = CCFileUtils::getInstance()->getWritablePath() + relativePath;
	FILE* fp = fopen(strPath.c_str(), "w");
#else
	vector <string> paths = DecomposePath(relativePath);
	for (int k = 0; k < SZ(paths); k++) if (!isFileExist(paths.at(k)))
		CreateDownloadedDir(paths.at(k));
	string fullPath = CCFileUtils::getInstance()->fullPathForFilename(relativePath);
	FILE *fp = fopen(fullPath.c_str(), "w");
#endif

	CCASSERT(fp != nullptr, "OpenFileW failed");
	return fp;
}
