//
//  MainScene.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/9/29.
//
//

#include "MainScene.h"

Scene* MainScene::createScene(){
    auto scene = Scene::create();
    
    //layer create
    
    //add layer
    
    return scene;
}

//////////////////////
//Main menu ui Layer//
//////////////////////
MainMenuLayer::MainMenuLayer(){
    
}

bool MainMenuLayer::init(){
    bool ret = false;
    
    //Super init
    if(!Layer::init())
        return false;
    
    return ret;
}

//////////////////////