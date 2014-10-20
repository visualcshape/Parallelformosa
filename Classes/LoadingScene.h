//
//  LoadingScene.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/9/5.
//
//

#ifndef Parallelformosa_Cocos2dx_LoadingScene_h
#define Parallelformosa_Cocos2dx_LoadingScene_h

#include <cocos2d.h>
#include <CocosGUI.h>
#include "CCPomeloWrapper.h"
#include "ConnectingSign.h"
#include "json.h"
#include "Database.h"
#include "globalVariable.h"
#include <fstream>
#include "PlayerManager.h"
#include <HttpClient.h>
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <copyfile.h>
#include <dirent.h>
#endif
#include "UtilFunc.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;
using namespace cocos2d::network;

class LoadingScene : public Layer
{
private:
protected:
public:
    static Scene* createScene();
};

//Loading bar Layer
class LoadingLayer:public Layer{
private:
    Text* _loadingItemText;
    LoadingBar* _loadingBar;
    
    //Datas
    std::vector<std::string> _fileNames;
    int _spriteCount;
    int _loadedSprite;
    string _uid;
    string _downloadPath;
    string _infoFileName;
    string _infoFileNameWithoutPostfix;
    
    //Start load
    void _startLoadUI();
    void _startLoadWindow();
    void _startLoadBuildingType();
    void _startLoadUnitType();
	void _startLoadMISC();
    //reset progress parameters such as loadedSprite and string
    void _startConnectServer();
    void _resetParameters();
    
    //copy file to writeable
    void _copyTMXandInfoToWriteablePath();
    
    //network
    void _sendRequest();
    void _requestCallback(const CCPomeloRequestResult& result);
    void _downloadInfoFile(string downloadPath,Json::Value user);
    
    void onHttpRequestComplete(HttpClient* sender,HttpResponse* resp);
    
    //Cal Progress
    double _calculateProgress();
    
    //sprintf
    std::string _sprintfProgress(std::string text,double progress);
    
    //Load complete
    void _loadComplete();
    
    //sql callback
    static int _uidQueryCallback(void* para,int columns,char** columnValue,char** columnName);
    
    //
    void _checkLoadComplete(void);
    
    //sync sqlite3
    void _writeUserToDB(Json::Value user);
    
protected:
public:
    LoadingLayer();
    ~LoadingLayer();
    
    virtual bool init();
    
    //custom create function
    static LoadingLayer* create();
    
    //loadcallback
    void loadingUICallback(Texture2D* texture);
    
    void loadingWindowCallback(Texture2D* texture);
    
    void loadingBuildingTypeCallback(Texture2D* texture);
    
    void loadingBuildingModelCallback();
    
    void loadingUnitTypeTextureCallback(Texture2D* texture);
    
    void loadingUnitTypeModelCallback();
};

#endif
