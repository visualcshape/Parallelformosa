//
//  LoadingScene.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/9/5.
//
//

#include "LoadingScene.h"
#include "VisibleRect.h"

Scene* LoadingScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = LoadingScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool LoadingScene::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    auto label = LabelTTF::create("Loading", "Arial", 24);
    label->setPosition(Vec2(VisibleRect::getVisibleRect().size.width/2, VisibleRect::getVisibleRect().size.height/2));
    addChild(label);
    
    return true;
}