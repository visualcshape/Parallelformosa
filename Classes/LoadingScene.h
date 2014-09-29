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
#include <CocosGUI.h>

USING_NS_CC;
using namespace cocos2d::ui;

class LoadingScene : public Layer
{
private:
protected:
public:
    static Scene* createScene();
};

//Loading bar Layer
class LoadingLayer:public Layer{
private:
    Text* _loadingItemText;
    LoadingBar* _loadingBar;
    
    //Datas
    std::vector<std::string> _fileNames;
    int _spriteCount;
    int _loadedSprite;
    
    //Start load
    void _startLoad();
protected:
public:
    LoadingLayer();
    ~LoadingLayer();
    
    virtual bool init();
    
    //custom create function
    static LoadingLayer* create();
    
    //loadcallback
    void loadingCallback(Texture2D* texture);
};

#endif
