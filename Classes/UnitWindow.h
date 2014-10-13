//
//  UnitWindow.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/12.
//
//

#ifndef __Parallelformosa_Cocos2dx__UnitWindow__
#define __Parallelformosa_Cocos2dx__UnitWindow__

#include <cocos2d.h>
#include "WindowProtocol.h"
#include "UnitTypeModel.h"
#include "ButtonWithImage.h"
#include "AppMacro.h"
#include "LayoutFactory.h"

USING_NS_CC;
USING_NS_CC::ui;
using namespace std;

class UnitWindow:public WindowProtocol
{
private:
    std::function<void(Ref*,Widget::TouchEventType)> _trainButtonPressedCallback;
    
    Layout* _unitMainLayout;
    Layout* _leftUnitLayout;
    Layout* _rightUnitLayout;
    
    ListView* _unitButtonListView;
    
protected:
    UnitWindow(string titleText,function<void(Ref*,Widget::TouchEventType)> closeCallback,function<void(Ref*,Widget::TouchEventType)> trainButtonPressedCallback);
public:
    ~UnitWindow();
    
    static UnitWindow* create(string titleText,function<void(Ref*,Widget::TouchEventType)> closeCallback,function<void(Ref*,Widget::TouchEventType)> trainButtonCallback);
    
    virtual bool init();
    
    void UnitButtonPressedCallback(Ref* pSender,Widget::TouchEventType type);
    
    //set focus not now
};

#endif /* defined(__Parallelformosa_Cocos2dx__UnitWindow__) */
