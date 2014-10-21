//
//  NoticeBox.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/22.
//
//

#include "NoticeBox.h"
#include "AppMacro.h"
#include "VisibleRect.h"

NoticeBox::NoticeBox(string title,string content)
{
    _title = title;
    _content = content;
}

NoticeBox::~NoticeBox()
{
    
}

NoticeBox* NoticeBox::create(string title, string content)
{
    NoticeBox* ret = new NoticeBox(title,content);
    if(ret&&ret->init())
    {
        ret->autorelease();
        return ret;
    }
    
    CC_ASSERT(false);
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool NoticeBox::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = VisibleRect::getVisibleRect().size;
    
    auto spriteFrameCache = SpriteFrameCache::getInstance();
    spriteFrameCache->addSpriteFramesWithFile("UI/busy.plist", "UI/busy.png");
    
    int spriteFrameCount = 60;
    char tmpStr[256];
    Vector<SpriteFrame*> animFrame(spriteFrameCount);
    for(int i = 1 ; i <= spriteFrameCount ; i++)
    {
        sprintf(tmpStr, "Busy00%02d.png",i);
        SpriteFrame* frame = spriteFrameCache->getSpriteFrameByName(tmpStr);
        animFrame.pushBack(frame);
    }
    
    auto animation = Animation::createWithSpriteFrames(animFrame);
    animation->setDelayPerUnit(0.03f);
    animFrame.clear();
    
    auto animate = Animate::create(animation);
    Action* act = RepeatForever::create(animate);
    
    //set position after content
    auto animatedSprite = Sprite::create();
    auto frame = spriteFrameCache->getSpriteFrameByName("Busy0001.png");
    animatedSprite->setSpriteFrame(frame);
    
    const char* fontName = "fonts/Silom.ttf";
    float fontSize = computeFontSize(24);
    
    _titleText = Text::create(_title, fontName, fontSize);
    CC_ASSERT(_titleText!=nullptr);
    
    _contentText = Text::create(_content,fontName,computeFontSize(20));
    CC_ASSERT(_contentText!=nullptr);
    
    float backgroundWidth = (visibleSize.width/10)*7;
    float backgroundHeight = _titleText->getContentSize().height+_contentText->getContentSize().height+10+30;
    Sprite* background = Sprite::create();
    background->setColor(Color3B(167,219,216));
    background->setOpacity(255*0.5);
    background->setContentSize(Size(backgroundWidth,backgroundHeight));
    background->setTextureRect(Rect(0, 0, backgroundWidth, backgroundHeight));
    background->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    CC_ASSERT(background!=nullptr);
    
    _titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _titleText->setPosition(Vec2(backgroundWidth/2, backgroundHeight-10));
    
    _contentText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _contentText->setPosition(Vec2(backgroundWidth/2, 10));
    
    animatedSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    animatedSprite->setPosition(Vec2(_contentText->getPosition().x-_contentText->getContentSize().width/2-10,_contentText->getPosition().y+_contentText->getContentSize().height/2));
    
    background->addChild(_titleText);
    background->addChild(animatedSprite);
    background->addChild(_contentText);
    addChild(background);
    
    
    animatedSprite->runAction(act);
    
    return true;
}