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

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

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
    
    //Start load
    void _startLoadUI();
    void _startLoadWindow();
    void _startLoadBuildingType();
    void _startLoadUnitType();
	void _startLoadMISC();
    //reset progress parameters such as loadedSprite and string
    void _startConnectServer();
    void _resetParameters();
    
    //network
    void _sendRequest();
    void _requestCallback(const CCPomeloRequestResult& result);
    
    //Cal Progress
    double _calculateProgress();
    
    //sprintf
    std::string _sprintfProgress(std::string text,double progress);
    
    //Load complete
    void _loadComplete();
    
    //sql callback
    
    void _checkLoadComplete(void);
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
