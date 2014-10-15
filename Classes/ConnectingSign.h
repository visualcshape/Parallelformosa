//
//  ConnectingSign.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/15.
//
//

#ifndef __Parallelformosa_Cocos2dx__ConnectingSign__
#define __Parallelformosa_Cocos2dx__ConnectingSign__

#include <cocos2d.h>
#include <CocosGUI.h>
#include <AppMacro.h>

USING_NS_CC;
USING_NS_CC::ui;

class ConnectingSign:public Layer
{
private:
    Text* _title;
    
    //data
    Color3B _color;
    std::string _text;
protected:
    ~ConnectingSign();
public:
    ConnectingSign(std::string text,Color3B color);
    
    static ConnectingSign* create(std::string text,Color3B textColor);
    
    virtual bool init();
    
    void setText(std::string text);
    
    void setTextColor(Color3B color);
};

#endif /* defined(__Parallelformosa_Cocos2dx__ConnectingSign__) */
