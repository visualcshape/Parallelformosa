//
//  Effect.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/11.
//
//

#ifndef __Parallelformosa_Cocos2dx__Effect__
#define __Parallelformosa_Cocos2dx__Effect__

#include <cocos2d.h>

USING_NS_CC;

class EffectSprite;

class Effect:public Ref
{
protected:
    GLProgramState* _glProgramState;
    bool initGLProgramState(const std::string& fragmentFilename);
    Effect();
    virtual ~Effect();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string _fragSource;
    EventListenerCustom* _backgroundListener;
#endif
public:
    GLProgramState* getGLProgramState()const{return _glProgramState;} ;
    virtual void setTarget(EffectSprite* sprite){}
};


//Effect sprite class
class EffectSprite:public Sprite
{
public:
};
//

//Effect class
class EffectOutline:public Effect
{
public:
    CREATE_FUNC(EffectOutline);
    
    bool init();
    
    virtual void setTarget(EffectSprite* sprite)override;
};
#endif /* defined(__Parallelformosa_Cocos2dx__Effect__) */
