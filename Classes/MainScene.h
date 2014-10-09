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
#include <CocosGUI.h>
#include "Observer.h"
#include "Subject.h"
#include "MainUIInfoModel.h"
#include "BuildingWindow.h"
#include "ButtonWithImage.h"

USING_NS_CC;
using namespace cocos2d::ui;

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
    Layout* _mainLayout;
    Button* _buildingButton;
    Button* _unitButton;
    Button* _statusButton;
    Button* _itemButton;
    Button* _alliesButton;
    
    //----Other----//
    Button* _optionButton;
protected:
public:
    virtual bool init();
    
    MainMenuLayer();
    
    void BuildingButtonCallback(Ref* pSender,Widget::TouchEventType type);
    void UnitButtonCallback(Ref* pSender,Widget::TouchEventType type);
    
    void ItemButtonCallback(Ref* pSender,Widget::TouchEventType type);
    
    void StatusButtonCallback(Ref* pSender,Widget::TouchEventType type);
    
    void AlliesButtonCallback(Ref* pSender,Widget::TouchEventType type);
    
    void OptionButtonCallback(Ref* pSender,Widget::TouchEventType type);
    
    CREATE_FUNC(MainMenuLayer);
};

//infomation (Upper)
class MainInfoLayer : public Layer,public Observer{
private:
    //Data
    MainUIInfoModel* _bindModel;
    //Components
    Label* _scrollingLabel;
    Sprite* _infoNoticeBackground;
    
    //model chaged
    void _scrollingTextModelChanged();
    //Update scrolling text position.
    void _scroll(float delta);
protected:
public:
    MainInfoLayer();
    ~MainInfoLayer();
    
    virtual bool init();
    
    virtual void Update(Subject* changedSubject);
    
    CREATE_FUNC(MainInfoLayer);
};
#endif /* defined(__Parallelformosa_Cocos2dx__MainScene__) */
