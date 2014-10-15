//
//  WindowProtocol.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/1.
//
//

#ifndef __Parallelformosa_Cocos2dx__WindowProtocol__
#define __Parallelformosa_Cocos2dx__WindowProtocol__

#include <cocos2d.h>
#include <CocosGUI.h>
#include <cocos-ext.h>
#include "VisibleRect.h"
#include "ButtonWithImage.h"

USING_NS_CC;
USING_NS_CC::ui;
USING_NS_CC::extension;

class WindowProtocol : public Layer{
private:
    //Component
    Button* _closeButton;
    Text* _titleText;
    
protected:
    //Data
    std::string _titleString;
    std::function<void(Ref*,Widget::TouchEventType)> _closeCallback;
    //layout
    Layout* _baseLayout;
    Layout* _titleLayout;
    Scale9Sprite* _baseWindow;
public:
    WindowProtocol(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback);
    ~WindowProtocol();
    
    virtual bool init();
    
    static WindowProtocol* create(std::string titleText,std::function<void(Ref*,Widget::TouchEventType)> closeCallback);
    
    virtual ButtonWithImage* getCurButton();
};

#endif /* defined(__Parallelformosa_Cocos2dx__WindowProtocol__) */
