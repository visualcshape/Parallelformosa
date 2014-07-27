//
//  TrademarkScene.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/27.
//
//

#include "TrademarkScene.h"

USING_NS_CC;

Scene* TrademarkScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = TrademarkScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool TrademarkScene::init()
{
    //Super init first
    if(!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    m_tradeMark = Sprite::create("Mark.png");
    CC_ASSERT(m_tradeMark!=NULL);
    m_tradeMark->setAnchorPoint(Vec2(0.5,0.5));
    m_tradeMark->setPosition(Vec2(visibleSize.width/2+origin.x,visibleSize.height/2+origin.y));
    m_tradeMark->setOpacity(0);
    
    this->addChild(m_tradeMark,0);
    
    //Call animation
    animate();
    
    return true;
}

void TrademarkScene::animate()
{
    animateTrademark();
}

void TrademarkScene::animateTrademark()
{
    float timeToFadeIn = 1.0f;
    float timeToFadeOut = 1.0f;
    
    auto fadeIn = FadeIn::create(timeToFadeIn);
    auto fadeOut = FadeOut::create(timeToFadeOut);
    
    auto sequence = Sequence::create(fadeIn,DelayTime::create(2.0f),fadeOut, NULL);
    
    m_tradeMark->runAction(sequence);
}