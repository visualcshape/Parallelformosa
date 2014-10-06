//
//  BuildingWindow.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/4.
//
//

#include "BuildingWindow.h"

BuildingWindow* BuildingWindow::create(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback, std::function<void()> windowDismissCallback){
    BuildingWindow* ret = new BuildingWindow(titleText,closeCallback,windowDismissCallback);
    if(ret&&ret->init()){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

BuildingWindow::BuildingWindow(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback,std::function<void()> windowDismissCallback):WindowProtocol(titleText,closeCallback){
    _windowDismissCallback = windowDismissCallback;
}

BuildingWindow::~BuildingWindow(){
    
}

bool BuildingWindow::init(){
    bool ret = false;
    
    //super init
    if(!WindowProtocol::init()){
        return false;
    }
    
    //Load Building data first...
    //...
    //
    
    //Layout
    Size baseSize = WindowProtocol::_baseLayout->getContentSize()-WindowProtocol::_titleLayout->getContentSize();
    _buildingMainLayout = Layout::create();
    CCASSERT(_buildingMainLayout!=nullptr, "_buildMainLayout");
    
    _scrollView = ui::ScrollView::create();
    //scrollView size depends on # of button
    
    return ret;
}
