//
//  TrademarkScene.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/27.
//
//

#ifndef Parallelformosa_Cocos2dx_TrademarkScene_h
#define Parallelformosa_Cocos2dx_TrademarkScene_h

#include <cocos2d.h>

class TrademarkScene : public cocos2d::Layer
{
private:
    cocos2d::Sprite* m_tradeMark;
protected:
    void animateTrademark();
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void animate();
    
    CREATE_FUNC(TrademarkScene);
    
};

#endif
