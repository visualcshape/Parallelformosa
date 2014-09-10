//
//  NoticeLoading.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/9/10.
//
//

#include "NoticeLoading.h"
#include "VisibleRect.h"

NoticeLoading::NoticeLoading()
{
    
}

NoticeLoading::~NoticeLoading()
{
    
}

NoticeLoading* NoticeLoading::create(std::string content, cocos2d::Color3B contentColor)
{
    NoticeLoading* ret = new NoticeLoading();
    
    ret->setContentColor(contentColor);
    ret->setContentText(content);
    ret->init();
    
    return ret;
}

bool NoticeLoading::init()
{
    bool ret = false;
    
    //Super init base..
    if(!Layer::init())
        return false;
    
    auto frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("UI/connecting.plist");
    
    //make animation
    int countAnimFrame = 77;
    cocos2d::Vector<SpriteFrame*> animFrames(countAnimFrame);
    char str[100] = {0};
    for(int i = 0 ; i < countAnimFrame ; i++)
    {
        sprintf(str, "connecting_Scene 1/00%02d",i);
        SpriteFrame* frame = frameCache->getSpriteFrameByName(str);
        animFrames.pushBack(frame);
    }
    
    auto animation = Animation::createWithSpriteFrames(animFrames);
    animation->setDelayPerUnit(0.03f);
    animFrames.clear();
    
    auto animate = Animate::create(animation);
    Action* act = RepeatForever::create(animate);
    //run after add child
    
    //Block
    _pBlockLayer = LayerColor::create(Color4B(0, 0, 0, 255*0.75), VisibleRect::getVisibleRect().size.width, VisibleRect::getVisibleRect().size.height);
    _pBlockLayer->setAnchorPoint(Vec2::ZERO);
    _pBlockLayer->setPosition(Vec2(0,0));
    
    auto blockListener = EventListenerTouchOneByOne::create();
    blockListener->setSwallowTouches(true);
    blockListener->onTouchBegan = [](Touch* touch,Event* e)
    {
        CCLOG("touch");
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(blockListener, _pBlockLayer);
    //
    
    //Text
    _pContentText = Text::create(_content, "fonts/Avenir.ttf", computeFontSize(45));
    _pContentText->setAnchorPoint(Vec2(1.0f, 1.0f));
    _pContentText->setPosition(Vec2(VisibleRect::getVisibleRect().size.width,VisibleRect::getVisibleRect().size.height));
    
    Size textSize = _pContentText->getContentSize();
    Vec2 textPos = _pContentText->getPosition();
    
    //sprite
    auto animatedSprite = Sprite::create();
    auto frame = frameCache->getSpriteFrameByName("connecting_Scene 1/0000");
    animatedSprite->setSpriteFrame(frame);
    animatedSprite->setAnchorPoint(Vec2(1.0f,0.5f));
    animatedSprite->setPosition(Vec2(textPos.x-textSize.width-10, textPos.y-textSize.height/2));
    
    //addChild
    addChild(_pBlockLayer,0);
    addChild(_pContentText,1);
    addChild(animatedSprite,1);
    
    //run action
    animatedSprite->runAction(act);
    
    ret = true;
    return ret;
}

void NoticeLoading::setContentText(std::string content)
{
    _content = content;
}

void NoticeLoading::setContentColor(cocos2d::Color3B contentColor)
{
    _contentColor = contentColor;
}