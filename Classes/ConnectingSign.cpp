//
//  ConnectingSign.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/15.
//
//

#include "ConnectingSign.h"
#include "VisibleRect.h"

ConnectingSign::ConnectingSign(std::string text,Color3B color):
_text(text),
_color(color),
_title(nullptr)
{
}

ConnectingSign::~ConnectingSign()
{
    
}

ConnectingSign* ConnectingSign::create(std::string text,Color3B color)
{
    ConnectingSign* pRet = new ConnectingSign(text,color);
    if(pRet&&pRet->init())
    {
        pRet->autorelease();
#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
        pRet->retain();
#endif
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool ConnectingSign::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    auto frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("UI/connectionSign.plist");
    
    int countAniFrame = 7;
    char aniName[10];
    cocos2d::Vector<SpriteFrame*> animFrameVector;
    for(int i = 0 ; i < countAniFrame ; i++)
    {
        sprintf(aniName, "%d.png",i);
        SpriteFrame* frame = frameCache->getSpriteFrameByName(aniName);
        animFrameVector.pushBack(frame);
    }
    
    auto animation = Animation::createWithSpriteFrames(animFrameVector);
    animation->setDelayPerUnit(0.3f);
    animFrameVector.clear();
    
    auto animate = Animate::create(animation);
    Action* act = RepeatForever::create(animate);
    
    Layer* block = LayerColor::create(Color4B(0, 0, 0, 25),VisibleRect::getVisibleRect().size.width,VisibleRect::getVisibleRect().size.height);
    block->setAnchorPoint(Vec2::ZERO);
    block->setPosition(Vec2::ZERO);
    
    auto blockListener = EventListenerTouchOneByOne::create();
    blockListener->setSwallowTouches(true);
    blockListener->onTouchBegan = [](Touch* t,Event* e)
    {
        return true;
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(blockListener, block);
    
    _title = Text::create(_text, "fonts/Silom.ttf", computeFontSize(36));
    _title->setAnchorPoint(Vec2(1.0f, 1.0f));
    _title->setPosition(Vec2(VisibleRect::getVisibleRect().size.width, VisibleRect::getVisibleRect().size.height));
    _title->setColor(_color);
    
    Size textSize = _title->getContentSize();
    Vec2 textPos = _title->getPosition();
    
    auto animatedSprite = Sprite::create();
    auto frame = frameCache->getSpriteFrameByName("0.png");
    animatedSprite->setSpriteFrame(frame);
    animatedSprite->setAnchorPoint(Vec2(1.0,0.5));
    animatedSprite->setPosition(Vec2(textPos.x-textSize.width-10, textPos.y-textSize.height/2));
    animatedSprite->setScale(.4, .4);
    
    addChild(block,0);
    addChild(_title,1);
    addChild(animatedSprite,1);
    
    animatedSprite->runAction(act);
    
    return true;
}

void ConnectingSign::setText(std::string text)
{
    _text = text;
    _title->setString(_text);
}

void ConnectingSign::setTextColor(Color3B color)
{
    _color = color;
    _title->setColor(_color);
}