//
//  WeatherLayer.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/18.
//
//

#include "WeatherLayer.h"
/////////////////
//WeatherLayer///
/////////////////

WeatherLayer::WeatherLayer()
{
    
}

WeatherLayer::~WeatherLayer()
{
    
}

bool WeatherLayer::init()
{
    _rainLayer=LayerColor::create(Color4B(155,159,168,255*0.25));
    CC_ASSERT(_rainLayer!=nullptr);
    
    _emitter = ParticleRain::create();
    _emitter->retain();
    
    //_emitter->setPosition(Vec2(VisibleRect::getVisibleRect().size.width/2,VisibleRect::getVisibleRect().size.height));
    _emitter->setLife(20);
    _emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("UI/rain.png"));
    
    addChild(_emitter,10);
    addChild(_rainLayer,9);
    
    return true;
}