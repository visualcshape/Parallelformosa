//
//  BuildingWindow.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/4.
//
//

#ifndef __Parallelformosa_Cocos2dx__BuildingWindow__
#define __Parallelformosa_Cocos2dx__BuildingWindow__

#include "WindowProtocol.h"
#include <cocos2d.h>
#include <CocosGUI.h>
#include "AppMacro.h"
#include "ButtonWithImage.h"
#include "BuildingType.h"
#include "BuildingModel.h"
#include <map>

USING_NS_CC;
USING_NS_CC::ui;

class BuildingWindow : public WindowProtocol{
private:
    std::function<void()> _windowDismissCallback;
    Layout* _buildingMainLayout;
    Layout* _buildingLeftLayout;
    Layout* _buildingRightLayout;
    ListView* _buildingButtonsListView;
    
    static float MARGIN;
protected:
    BuildingWindow(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback,std::function<void()> windowDismissCallback);
public:
    ~BuildingWindow();
    //Factory
    static BuildingWindow* create(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback, std::function<void()> windowDismissCallback);
    
    virtual bool init();
    
    void buildingButtonCallback(Ref* pSender,Widget::TouchEventType);
};

#endif /* defined(__Parallelformosa_Cocos2dx__BuildingWindow__) */
