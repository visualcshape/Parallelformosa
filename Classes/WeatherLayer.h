//
//  WeatherLayer.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/18.
//
//

#ifndef __Parallelformosa_Cocos2dx__WeatherLayer__
#define __Parallelformosa_Cocos2dx__WeatherLayer__

#include <cocos2d.h>
#include "VisibleRect.h"

USING_NS_CC;
using namespace std;

class WeatherLayer:public Layer
{
private:
    Layer* _rainLayer;
    ParticleRain* _emitter;
protected:
public:
    WeatherLayer();
    ~WeatherLayer();
    
    virtual bool init();
    
    CREATE_FUNC(WeatherLayer);
    
};

#endif /* defined(__Parallelformosa_Cocos2dx__WeatherLayer__) */
