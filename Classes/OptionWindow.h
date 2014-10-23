//
//  OptionWindow.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/20.
//
//

#ifndef __Parallelformosa_Cocos2dx__OptionWindow__
#define __Parallelformosa_Cocos2dx__OptionWindow__

#include <cocos2d.h>
#include <CocosGUI.h>
#include "WindowProtocol.h"
#include "VisibleRect.h"
#include "AppMacro.h"

USING_NS_CC;
USING_NS_CC::ui;
using namespace std;

class OptionWindow:public WindowProtocol
{
private:
    function<void(Ref*,Widget::TouchEventType)> _resetButtonPressedCallback;
    
    Button* _resetButton;
    CheckBox* _musicEnabledCheckBox;
    Layout* _compLayout;
protected:
    OptionWindow(string title,function<void(Ref*,Widget::TouchEventType)> closeCallback,function<void(Ref*,Widget::TouchEventType)> resetButtonPressedCallback);
public:
    ~OptionWindow();
    
    static OptionWindow* create(string title,function<void(Ref*,Widget::TouchEventType)> closeCallback,function<void(Ref*,Widget::TouchEventType)> resetButtonPressedCallback);
    
    void onMusicEnabledCheckBoxCallback(Ref* pSender,CheckBox::EventType type);
    
    void onResetButtonClick(Ref* pSender,Widget::TouchEventType type);
    
    virtual bool init();
    
};

#endif /* defined(__Parallelformosa_Cocos2dx__OptionWindow__) */
