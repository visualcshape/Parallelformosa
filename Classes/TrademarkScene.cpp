//
//  TrademarkScene.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/27.
//
//

#include "TrademarkScene.h"
#include "LoginTitleScene.h"
#include "VisibleRect.h"

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
    
    //?
    m_tradeMark = Sprite::create("Trademark/Mark.png");
    CC_ASSERT(m_tradeMark!=NULL);
    m_tradeMark->setAnchorPoint(Vec2(0.5,0.5));
    m_tradeMark->setPosition(Vec2(visibleSize.width/2+origin.x,visibleSize.height/2+origin.y));
    m_tradeMark->setOpacity(0);
    
    //A trabsparentSprite
    auto transparentSprite = Sprite::create();
    CC_ASSERT(transparentSprite!=nullptr);
    transparentSprite->setTextureRect(Rect(VisibleRect::getVisibleRect().origin.x,VisibleRect::getVisibleRect().origin.y,VisibleRect::getVisibleRect().size.width,VisibleRect::getVisibleRect().size.height));
    
    //Add transparent sprite as menu item
    m_transparentSpriteMenuItem = MenuItemSprite::create(transparentSprite, NULL);
    CC_ASSERT(m_transparentSpriteMenuItem!=NULL);
    m_transparentSpriteMenuItem->setCallback(CC_CALLBACK_0(TrademarkScene::replaceScene, this));
    m_transparentSpriteMenuItem->setOpacity(0);
    
    //Menu for transparents sprite
    m_menu = Menu::create(m_transparentSpriteMenuItem, NULL);
    CC_ASSERT(m_menu!=nullptr);
    addChild(m_menu,0);
    
    addChild(m_tradeMark,1);
    
    //Call animation
    animate();
    
    //Console : Output Size
    CCLOG("%f, %f",visibleSize.width,visibleSize.height);
    
    return true;
}

void TrademarkScene::animate()
{
    animateTrademark();
}

void TrademarkScene::animateTrademark()
{
    float timeToFadeIn = 1.0f;
    float timeToFadeOut = 2.0f;
    
    auto fadeIn = FadeIn::create(timeToFadeIn);
    auto fadeOut = FadeOut::create(timeToFadeOut);

    auto callFunc = CallFunc::create(CC_CALLBACK_0(TrademarkScene::replaceScene, this));
    
    auto sequence = Sequence::create(fadeIn,DelayTime::create(2.0f),fadeOut, callFunc, NULL);
    
    m_tradeMark->runAction(sequence);
}

void TrademarkScene::replaceScene()
{
    Scene* pNextScene = LoginTitleScene::createScene();
    CC_ASSERT(pNextScene!=NULL);
    auto fadeTransition = TransitionFade::create(0.32f, pNextScene, Color3B(0, 0, 0));
    
    Director::getInstance()->replaceScene(fadeTransition);
}