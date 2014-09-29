//
//  MainScene.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/9/29.
//
//

#ifndef __Parallelformosa_Cocos2dx__MainScene__
#define __Parallelformosa_Cocos2dx__MainScene__

#include <cocos2d.h>

USING_NS_CC;

//scene
class MainScene:public Layer{
private:
protected:
public:
    static Scene* createScene();
};

//menu ui
class MainMenuLayer : public Layer{
private:
protected:
public:
    virtual bool init();
    
    MainMenuLayer();
    
    CREATE_FUNC(MainMenuLayer);
};
#endif /* defined(__Parallelformosa_Cocos2dx__MainScene__) */
