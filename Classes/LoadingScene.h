//
//  LoadingScene.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/9/5.
//
//

#ifndef Parallelformosa_Cocos2dx_LoadingScene_h
#define Parallelformosa_Cocos2dx_LoadingScene_h

#include <cocos2d.h>

USING_NS_CC;

class LoadingScene : public Layer
{
private:
protected:
public:
    static Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(LoadingScene);
};

#endif
