//
//  LoadingScene.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/9/5.
//
//

#include "LoadingScene.h"
#include "VisibleRect.h"
#include "AppMacro.h"
#include <fstream>
#include <math.h>
#include "MainScene.h"
#include "BuildingModel.h"
#include "UnitTypeModel.h"
#include "ResourceModel.h"
#include "SceneManager.h"

Scene* LoadingScene::createScene()
{
	auto scene = Scene::create();

	//init layer
	auto loadingLayer = LoadingLayer::create();
	//add child
	scene->addChild(loadingLayer);

	return scene;
}

/////////////////
//Loading Layer//
/////////////////
LoadingLayer::LoadingLayer(){
	//Load file list
	std::ifstream inputStream;
	std::string curLine;
	std::string contents;
	long fSize = 0;
	std::string fullPath = FileUtils::getInstance()->fullPathForFilename("UI/MainSceneResourceList.txt");
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	unsigned char* buf = FileUtils::getInstance()->getFileData(fullPath.c_str(), "r", &fSize);
	contents.append((char*)buf);

	std::istringstream fileStream(contents);

	while (std::getline(fileStream, curLine)){
		_fileNames.push_back(curLine);
	}
#else
	inputStream.open(fullPath.c_str());
	CCASSERT(inputStream.is_open(), "Cannot open file UI/MainSceneResourceList.txt");
	while (std::getline(inputStream, curLine)){
		_fileNames.push_back(curLine);
	}
#endif
	//pop useless last index
	_fileNames.pop_back();
	inputStream.close();
	_fileNames.shrink_to_fit();
	_spriteCount = (int)_fileNames.size();
	_loadedSprite = 0;
}

LoadingLayer::~LoadingLayer(){

}

LoadingLayer* LoadingLayer::create(){
	LoadingLayer* pRet = new LoadingLayer();
	if (pRet && pRet->init()){
		pRet->autorelease();
		return pRet;
	}
	else{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool LoadingLayer::init(){
	bool ret = false;

	//Super init first
	if (!Layer::init())
		return ret;

	//Text
	_loadingItemText = Text::create("Undefined", "fonts/Avenir.ttf", computeFontSize(24));
	_loadingItemText->setColor(Color3B(197, 224, 220));
	_loadingItemText->setPosition(Vec2(VisibleRect::getVisibleRect().size.width / 2, _loadingItemText->getContentSize().height));

	//loading bar
	_loadingBar = LoadingBar::create("UI/LoadingBar.png");
	CCASSERT(_loadingBar != nullptr, "\"LoadingBar\" cannot be null.");
	_loadingBar->setPosition(Vec2(VisibleRect::getVisibleRect().size.width / 2, _loadingItemText->getContentSize().height + _loadingBar->getContentSize().height + 10));

	this->addChild(_loadingItemText, 0);
	this->addChild(_loadingBar, 1);

	ret = true;
	_startLoadUI();
	return ret;
}

void LoadingLayer::loadingUICallback(cocos2d::Texture2D *texture){
	++_loadedSprite;
	char displayChar[64];
	double progress = ((double)_loadedSprite / (double)_spriteCount)*100.0;
	sprintf(displayChar, "Loading UI Components...(%.0f%%)", progress);
	_loadingItemText->setString(displayChar);
	_loadingBar->setPercent((float)progress);
	//Load complete
	if (_loadedSprite == _spriteCount){
		//LoadWindow
		_startLoadWindow();
	}
}

void LoadingLayer::_startLoadUI(){
	auto textureCache = Director::getInstance()->getTextureCache();

	//Load UI
	std::vector<std::string>::iterator it;
	_loadingItemText->setString("Loading UI Components...(0%)");
	for (it = _fileNames.begin(); it != _fileNames.end(); it++){
		textureCache->addImageAsync(*it, CC_CALLBACK_1(LoadingLayer::loadingUICallback, this));
	}

}

void LoadingLayer::_startLoadWindow(){
	auto textureCache = Director::getInstance()->getTextureCache();
	//Load Window
	_resetParameters();
	_spriteCount = 1;
	_loadingItemText->setString("Loading Windows Components...(0%)");
	textureCache->addImageAsync("UI/MainUI_Windows_Component.png", CC_CALLBACK_1(LoadingLayer::loadingWindowCallback, this));

}

void LoadingLayer::_startLoadBuildingType()
{
	auto textureCache = Director::getInstance()->getTextureCache();
	_resetParameters();
	//1 for resource 1 for building model.
	_spriteCount = 2;
	_loadingItemText->setString("Loading Building Type Components...(0%)");
	textureCache->addImageAsync("Building/BuildingType.png", CC_CALLBACK_1(LoadingLayer::loadingBuildingTypeCallback, this));
	auto cb = CC_CALLBACK_0(LoadingLayer::loadingBuildingModelCallback, this);
	BuildingModel::getInstance()->initModelAsync(cb);
}

void LoadingLayer::_startLoadMISC(){
	_resetParameters();

	_spriteCount = 6;
	_loadingItemText->setString("Loading MISC Components...(0%)");

	ResourceModel::getModel()->LoadBuildings();
	++_loadedSprite;
	_loadingItemText->setString(_sprintfProgress("Loading MISC Components...(%.0f%%)", _calculateProgress()));
	_loadingBar->setPercent((float)_calculateProgress());

	ResourceModel::getModel()->LoadTroops();
	++_loadedSprite;
	_loadingItemText->setString(_sprintfProgress("Loading MISC Components...(%.0f%%)", _calculateProgress()));
	_loadingBar->setPercent((float)_calculateProgress());

	ResourceModel::getModel()->LoadTerrains();
	++_loadedSprite;
	_loadingItemText->setString(_sprintfProgress("Loading MISC Components...(%.0f%%)", _calculateProgress()));
	_loadingBar->setPercent((float)_calculateProgress());

	ResourceModel::getModel()->LoadTilemaps();
	++_loadedSprite;
	_loadingItemText->setString(_sprintfProgress("Loading MISC Components...(%.0f%%)", _calculateProgress()));
	_loadingBar->setPercent((float)_calculateProgress());

	ResourceModel::getModel()->LoadPlayers();
	++_loadedSprite;
	_loadingItemText->setString(_sprintfProgress("Loading MISC Components...(%.0f%%)", _calculateProgress()));
	_loadingBar->setPercent((float)_calculateProgress());
	ResourceModel::getModel()->LoadMISC();
	++_loadedSprite;
	_loadingItemText->setString(_sprintfProgress("Loading MISC Components...(%.0f%%)", _calculateProgress()));
	_loadingBar->setPercent((float)_calculateProgress());

    _startConnectServer();
}

void LoadingLayer::_startLoadUnitType()
{
    _resetParameters();
    //1 for resource 1 fot unit type model.
    TextureCache* cache = Director::getInstance()->getTextureCache();
        _spriteCount = 2;
    _loadingItemText->setString("Loading Unit Type Components...(0%)");
    cache->addImageAsync("Unit/UnitType.png", CC_CALLBACK_1(LoadingLayer::loadingUnitTypeTextureCallback, this));
    //loading tetxure
    //
    
    auto initModelCallback = CC_CALLBACK_0(LoadingLayer::loadingUnitTypeModelCallback, this);
    UnitTypeModel::getInstance()->initModelAsync(initModelCallback);
}

void LoadingLayer::_startConnectServer()
{
    ConnectingSign* sign = ConnectingSign::create("Connect To Server...", Color3B(59, 134, 134));
    addChild(sign,10,"Sign");
    
    _resetParameters();
    CCPomeloWrapper* instance = CCPomeloWrapper::getInstance();
    CC_ASSERT((GlobalVariable::getInstance()->getConnectorHost())!="");
    CC_ASSERT(GlobalVariable::getInstance()->getConnectorPort()!=0);
    instance->connectAsnyc(GlobalVariable::getInstance()->getConnectorHost().c_str(), GlobalVariable::getInstance()->getConnectorPort(), [=](int err){
        if(err!=0)
        {
            //back to title use assert first
            CCLOG("Connect failure...");
        }
        else
        {
            CCLOG("Connect successfully.");
            _sendRequest();
        }
    });
}

void LoadingLayer::_sendRequest()
{
    std::string route = "connector.entryHandler.entry";
    auto instance = CCPomeloWrapper::getInstance();
    
    Json::Value msg;
    Json::FastWriter writer;
    
    sqlite3* pDB = Database::getInstance()->getDatabasePointer();
    //get uid
    string sql = "select * from User";
    string uid = "";
    string firstLogin = "";
    string pass[] = {uid,firstLogin};
    char* pErrMsg;
    
    int result = sqlite3_exec(pDB, sql.c_str(), LoadingLayer::_uidQueryCallback, pass, &pErrMsg);
    CC_ASSERT(result==SQLITE_OK);
    
    uid = pass[0];
    firstLogin = pass[1];
    
    CC_ASSERT(uid!="");
    CC_ASSERT(firstLogin!="");
    
    msg["uid"] = uid;
    msg["firstLogin"] = firstLogin;
    
    _uid = uid;
    
    instance->request(route.c_str(), writer.write(msg), CC_CALLBACK_1(LoadingLayer::_requestCallback, this));
    
}

void LoadingLayer::_requestCallback(const CCPomeloRequestResult& result)
{
    string infoDownloadPath = "";
    
    _loadingItemText->setString("Received Data,Syncing...");
    
    Json::Value root;
    Json::Reader reader;
    Json::Value user;
    if(reader.parse(result.jsonMsg, root))
    {
        CCLOG("Server response\n %s",root.toStyledString().c_str());
        infoDownloadPath = root["infoDownloadPath"].asString();
        user = root["user"];
        
        _downloadPath = infoDownloadPath;
        //Sync to SQLite3
        _writeUserToDB(user);
        _downloadInfoFile(infoDownloadPath,user);
    }
}

void LoadingLayer::_downloadInfoFile(string downloadPath,Json::Value user)
{
    _resetParameters();
    if(downloadPath.empty()){
        //create an empty info file
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        FILE* fp = fopen((FileUtils::getInstance()->getWritablePath()+_fileName).c_str(), "wb");
#else
        FILE* fp = fopen((FileUtils::getInstance()->getWritablePath()+"/"+user["OwnMapCoord"].asString()+".info").c_str(), "w");
        
#endif
        CC_ASSERT(fp!=nullptr);
        fputs(("s").c_str(), fp);
        
        _checkLoadComplete();
    }
    HttpRequest* req = new HttpRequest();
    req->setRequestType(HttpRequest::Type::GET);
    req->setUrl(downloadPath.c_str());
    req->setTag("Downloading TMX");
    req->setResponseCallback(CC_CALLBACK_2(LoadingLayer::onHttpRequestComplete, this));
    _loadingItemText->setString("Downloading TMX");
    HttpClient::getInstance()->send(req);
    req->release();
}

void LoadingLayer::onHttpRequestComplete(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *resp)
{
    if(!resp)
        return;
    _resetParameters();
    _spriteCount=0;
    _loadedSprite = 0;
    
    //get file name
    istringstream iss(_downloadPath);
    vector<string> tokens{istream_iterator<string>{iss},
        istream_iterator<string>{}};
    
    _fileName = tokens.at(tokens.size());
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    FILE* fp = fopen((FileUtils::getInstance()->getWritablePath()+_fileName).c_str(), "wb");
#else
    FILE* fp = fopen((FileUtils::getInstance()->getWritablePath()+"/"+_fileName).c_str(), "wb");
#endif
    
    std::vector<char>* buffer = resp->getResponseData();
    
    buffer->shrink_to_fit();
    _spriteCount = (int)buffer->size();
    
    for(unsigned int i = 0 ; i < buffer->size() ; i++)
    {
        ++_loadedSprite;
        _loadingItemText->setString(_sprintfProgress("Downloading Info (%.0f%%)",(float)_calculateProgress()));
        _loadingBar->setPercent((float)_calculateProgress());
        CCLOG("Loading:%d",i);
        fwrite(&buffer->at(i), 1, 1, fp);
    }
    fclose(fp);
    _checkLoadComplete();
}

void LoadingLayer::_writeUserToDB(Json::Value user)
{
    sqlite3* pDB = Database::getInstance()->getDatabasePointer();
    
    char sql[512];
    char* errMsg;
    sprintf(sql, "UPDATE User SET FirstTimeLogin='%s',GPower='%s',LMana='%s',Food='%s',OwnMapCoord='%s',FoodGenRate='%s',GPowerGenRate='%s',LManaGenRate='%s',FoodLevel='%s',GPowerLevel='%s',LManaLevel='%s',BarrackLevel='%s' WHERE ID='%s'" ,"0",user["GPower"].asString().c_str(),user["LMana"].asString().c_str(),user["Food"].asString().c_str(),user["OwnMapCoord"].asString().c_str(),user["FoodGenRate"].asString().c_str(),user["GPowerGenRate"].asString().c_str(),user["LManaGenRate"].asString().c_str(),user["FoodGenLevel"].asString().c_str(),user["GPoweGenLevel"].asString().c_str(),user["LManaGenRate"].asString().c_str(),user["BarrackLevel"].asString().c_str(),_uid.c_str());
    
    int result = sqlite3_exec(pDB, sql, nullptr, nullptr, &errMsg);
    CCASSERT(result==SQLITE_OK, errMsg);
    
    //start downloading
    return;
}

int LoadingLayer::_uidQueryCallback(void *para, int columns, char **columnValue, char **columnName)
{
    string* strArr = (string*)para;
    
    for(int i = 0 ; i < columns ; i++)
    {
        if(string(columnName[i])=="ID")
        {
            strArr[0] = string(columnValue[i]);
        }
        else if(string(columnName[i])=="FirstTimeLogin")
        {
            strArr[1] = string(columnValue[i]);
        }
    }
    
    return SQLITE_OK;
}

void LoadingLayer::_resetParameters(){
	_loadedSprite = 0;
	_spriteCount = 0;
	_loadingBar->setPercent(0.0f);
	_loadingItemText->setString("Pending...");
}

void LoadingLayer::loadingWindowCallback(cocos2d::Texture2D *texture){
	++_loadedSprite;
	//Add to sprite frame cache
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("UI/MainUI_Windows_Component.plist", texture);
	_loadingItemText->setString(_sprintfProgress("Loading Window Components...(%.0f%%)", _calculateProgress()));
	_loadingBar->setPercent((float)_calculateProgress());

	if (_loadedSprite == _spriteCount)
	{
		_startLoadBuildingType();
	}
}

void LoadingLayer::loadingBuildingTypeCallback(cocos2d::Texture2D *texture){
    ++_loadedSprite;
    
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("Building/BuildingType.plist", texture);
    _loadingItemText->setString(_sprintfProgress("Loading Building Type Components...(%.0f%%)", _calculateProgress()));
    _loadingBar->setPercent((float)_calculateProgress());
    
    //Attention
    if(_loadedSprite==_spriteCount)
    {
        _startLoadUnitType();
    }
}

void LoadingLayer::loadingBuildingModelCallback()
{
    //Attention//
    ++_loadedSprite;
    _loadingItemText->setString(_sprintfProgress("Loading Building Type Components...(%.0f%%)", _calculateProgress()));
    _loadingBar->setPercent((float)_calculateProgress());
    
    if(_loadedSprite==_spriteCount)
    {
        //Next step
        _startLoadUnitType();
    }
}

void LoadingLayer::loadingUnitTypeTextureCallback(Texture2D* texture)
{
    ++_loadedSprite;
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("Unit/UnitType.plist", texture);
    _loadingItemText->setString(_sprintfProgress("Loading Unit Type Components...(%.0f%%)", _calculateProgress()));
    _loadingBar->setPercent((float)_calculateProgress());
    
    
    if (_loadedSprite == _spriteCount)
    {
        CallFunc* f = CallFunc::create(CC_CALLBACK_0(LoadingLayer::_startLoadMISC, this));
        Sequence* seq = Sequence::create(DelayTime::create(1.0f), f,NULL);
        this->runAction(seq);
        //_startLoadMISC();
    }
}

void LoadingLayer::loadingUnitTypeModelCallback()
{
	//Attention//
	++_loadedSprite;
	_loadingItemText->setString(_sprintfProgress("Loading Unit Type Components...(%.0f%%)", _calculateProgress()));
	_loadingBar->setPercent((float)_calculateProgress());

	if (_loadedSprite == _spriteCount)
	{
        CallFunc* f = CallFunc::create(CC_CALLBACK_0(LoadingLayer::_startLoadMISC, this));
        Sequence* seq = Sequence::create(DelayTime::create(1.0f), f,NULL);
        this->runAction(seq);
		//_startLoadMISC();
	}
}


double LoadingLayer::_calculateProgress(){
	return ((double)_loadedSprite / (double)_spriteCount)*100.0;
}

std::string LoadingLayer::_sprintfProgress(std::string text, double progress){
	char ret[64];
	sprintf(ret, text.c_str(), progress);
	return ret;
}

void LoadingLayer::_loadComplete(){
	if (_loadedSprite == _spriteCount){
        
        PlayerManager::getInstance()->setCurPlayer(new PlayerModel());
        PlayerManager::getInstance()->setAtkPlayer(PlayerManager::getInstance()->getCurPlayer());
        PlayerManager::getInstance()->setDefPlayer(PlayerManager::getInstance()->getCurPlayer());
		ResourceModel *rm = ResourceModel::getModel();
		SceneManager::goMapScreen(rm->strWorldMap, HUD_ID::DEFENSE);
		/*
		auto scene = MainScene::createScene();
		Director::getInstance()->replaceScene(scene);*/
		CCLOG("[Loading Scene]Load complete.");
	}
	else{
		CCLOG("Not Loaded completed...");
	}
}

void LoadingLayer::_checkLoadComplete()
{
	if (_spriteCount == _loadedSprite)
	{
		_loadComplete();
	}
}
/////////////////
