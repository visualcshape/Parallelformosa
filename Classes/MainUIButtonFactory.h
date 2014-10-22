//
//  MainUIButtonFactory.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/9/30.
//
//

#ifndef __Parallelformosa_Cocos2dx__MainUIButtonFactory__
#define __Parallelformosa_Cocos2dx__MainUIButtonFactory__

#include <cocos2d.h>
#include <CocosGUI.h>

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

class MainUIButtonFactory{
private:
protected:
public:
    static Button* create(Vec2 position,std::function<void(Ref*,Widget::TouchEventType)> callback,string normal,string pressed);
};

#endif /* defined(__Parallelformosa_Cocos2dx__MainUIButtonFactory__) */
