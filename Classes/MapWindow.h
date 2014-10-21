//
//  MapWindow.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/20.
//
//

#ifndef __Parallelformosa_Cocos2dx__MapWindow__
#define __Parallelformosa_Cocos2dx__MapWindow__

#include <cocos2d.h>
#include <CocosGUI.h>
#include "WindowProtocol.h"
#include "AppMacro.h"

USING_NS_CC;
USING_NS_CC::ui;
using namespace std;

class MapWindow:public WindowProtocol
{
private:
    std::function<void(Ref*,Widget::TouchEventType)> _goMapButtonPressedCallback;
    
    TextField* _mapTextField;
    
    Layout* _compLayout;
    
    Button* _buttonGoMap;
protected:
    MapWindow(string title,function<void(Ref*,Widget::TouchEventType)> closeCallback,function<void(Ref*,Widget::TouchEventType)> goMapButtonPressedCallback);
public:
    ~MapWindow();
    
    static MapWindow* create(string title,function<void(Ref*,Widget::TouchEventType)> closeCallback,function<void(Ref*,Widget::TouchEventType)> goMapCallback);
    
    virtual bool init();
    
    string getTextFieldString();
};

#endif /* defined(__Parallelformosa_Cocos2dx__MapWindow__) */
