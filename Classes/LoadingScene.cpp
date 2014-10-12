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
    
    inputStream.open(FileUtils::getInstance()->fullPathForFilename("UI/MainSceneResourceList.txt"));
    CCASSERT(inputStream.is_open(), "Cannot open file UI/MainSceneResourceList.txt");
    
    while(std::getline(inputStream, curLine)){
        _fileNames.push_back(curLine);
    }
    inputStream.close();
    _fileNames.shrink_to_fit();
    _spriteCount = (int)_fileNames.size();
    _loadedSprite = 0;
}

LoadingLayer::~LoadingLayer(){
    
}

LoadingLayer* LoadingLayer::create(){
    LoadingLayer* pRet = new LoadingLayer();
    if(pRet && pRet->init()){
        pRet->autorelease();
        return pRet;
    }else{
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool LoadingLayer::init(){
    bool ret = false;
    
    //Super init first
    if(!Layer::init())
        return ret;
    
    //Text
    _loadingItemText = Text::create("Undefined", "fonts/Avenir.ttf", computeFontSize(24));
    _loadingItemText->setColor(Color3B(197,224,220));
    _loadingItemText->setPosition(Vec2(VisibleRect::getVisibleRect().size.width/2,_loadingItemText->getContentSize().height));
    
    //loading bar
    _loadingBar = LoadingBar::create("UI/LoadingBar.png");
    CCASSERT(_loadingBar!=nullptr, "\"LoadingBar\" cannot be null.");
    _loadingBar->setPosition(Vec2(VisibleRect::getVisibleRect().size.width/2, _loadingItemText->getContentSize().height+_loadingBar->getContentSize().height+10));
    
    this->addChild(_loadingItemText,0);
    this->addChild(_loadingBar,1);
    
    ret = true;
    _startLoadUI();
    return ret;
}

void LoadingLayer::loadingUICallback(cocos2d::Texture2D *texture){
    ++_loadedSprite;
    char displayChar[64];
    double progress = ((double)_loadedSprite/(double)_spriteCount)*100.0;
    sprintf(displayChar, "Loading UI Components...(%.0f%%)",progress);
    _loadingItemText->setString(displayChar);
    _loadingBar->setPercent((float)progress);
    //Load complete
    if(_loadedSprite==_spriteCount){
        //LoadWindow
        _startLoadWindow();
    }
}

void LoadingLayer::_startLoadUI(){
    auto textureCache = Director::getInstance()->getTextureCache();
    
    //Load UI
    std::vector<std::string>::iterator it;
    _loadingItemText->setString("Loading UI Components...(0%)");
    for(it = _fileNames.begin() ; it != _fileNames.end() ; it++){
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
    
    //Attention//
    _loadComplete();
}

double LoadingLayer::_calculateProgress(){
    return ((double)_loadedSprite/(double)_spriteCount)*100.0;
}

std::string LoadingLayer::_sprintfProgress(std::string text,double progress){
    char ret[64];
    sprintf(ret, text.c_str(), progress);
    return ret;
}

void LoadingLayer::_loadComplete(){
    if(_loadedSprite==_spriteCount){
        auto scene = MainScene::createScene();
        Director::getInstance()->replaceScene(scene);
        CCLOG("[Loading Scene]Load complete.");
    }else{
        CCLOG("Not Loaded completed...");
    }
}
/////////////////