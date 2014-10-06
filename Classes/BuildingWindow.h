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

USING_NS_CC;
USING_NS_CC::ui;

class BuildingWindow : public WindowProtocol{
private:
    std::function<void()> _windowDismissCallback;
    ui::ScrollView* _scrollView;
    Layout* _buildingMainLayout;
protected:
    BuildingWindow(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback,std::function<void()> windowDismissCallback);
public:
    ~BuildingWindow();
    //Factory
    static BuildingWindow* create(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback, std::function<void()> windowDismissCallback);
    
    virtual bool init();
};

#endif /* defined(__Parallelformosa_Cocos2dx__BuildingWindow__) */
