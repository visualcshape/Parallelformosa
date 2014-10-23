//
//  NetManager.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/18.
//
//

#include "NetManager.h"
#include "SceneManager.h"

NetManager* NetManager::_instance = nullptr;

string NetManager::DIALOGUE_NAME = "DialogueLostConnect";

NetManager::NetManager()
{
    
}

NetManager::~NetManager()
{
    
}

NetManager* NetManager::getInstance()
{
    if(_instance==nullptr)
    {
        _instance = new NetManager();
        return _instance;
    }
    
    return _instance;
}

void NetManager::onConnectLost()
{
    auto scene = Director::getInstance()->getRunningScene();
    DialogueWindowConfirm* pD = DialogueWindowConfirm::create("Error", Color3B::RED, "Lost connection with server.\nPress OK to return to title.", Color3B::BLACK);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    pD->retain();
#endif
    pD->addButtonListener(CC_CALLBACK_2(NetManager::OKCB, this));
    scene->addChild(pD,100,DIALOGUE_NAME);
}

void NetManager::OKCB(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        Director::getInstance()->getTextureCache()->removeAllTextures();
        SpriteFrameCache::getInstance()->destroyInstance();
        SceneManager::goTitleScreen();
        
        
        CCLOG("GoToTitle(STUB)");
    }
}
