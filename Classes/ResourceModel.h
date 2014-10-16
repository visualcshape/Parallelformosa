#pragma once
#include "cocos2d.h"
#include "AppMacro.h"


USING_NS_CC;

class ResourceModel :public Ref{
public:
	static const int BUILDING_SIZE = 5;
	static const int TROOP_SIZE = 5;
	static const int TERRAIN_SIZE = 11;
	static const int TILEMAP_SIZE = 9;
	static const int PALYER_SIZE = 2;

public:
	ResourceModel();
	~ResourceModel();
	void LoadBuildings();
	void LoadTroops();
	void LoadTerrains();
	void LoadTilemaps();
	void LoadPlayers();
	void LoadMISC();

	string getSystemTimeString();
	tm makeSystemTime(int year, int month, int day, int hour, int min, int sec);

	static ResourceModel* getModel();
	vector <string> DecomposePath(string relativePath); 
	string NoFolderPath(string relativePath);

	FILE* OpenFileR(string relativePath);
	FILE* OpenFileW(string relativePath);
	bool isFileExist(string pFileName);

	string strBuilding[BUILDING_SIZE + 1];
	string strTroop[TROOP_SIZE + 1];
	string strTerrain[TERRAIN_SIZE + 1];

	string strWorldMap;
	string strTileMap[TILEMAP_SIZE + 1];
	string strPlayerMap[PALYER_SIZE + 1];

	string strRangePic;
	string strHUDPic;

	int GIDBuilding[BUILDING_SIZE + 1];
	int GIDTroop[TROOP_SIZE + 1];
	int GIDTerrain[TERRAIN_SIZE + 1];

	int costLstr[TROOP_SIZE + 1];
	int costGmag[TROOP_SIZE + 1];
	int costFood[TROOP_SIZE + 1];

protected:
	static ResourceModel* rm_pInstance;

private:
	void CreateDownloadedDir(string relativePath);
	void copyData(string pFileName);
};