//
//  NoticeBox.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/22.
//
//

#ifndef __Parallelformosa_Cocos2dx__NoticeBox__
#define __Parallelformosa_Cocos2dx__NoticeBox__

#include <cocos2d.h>
#include <CocosGUI.h>

USING_NS_CC;
USING_NS_CC::ui;

using namespace std;

class NoticeBox:public Layer
{
private:
    //no callback
    Text* _titleText;
    Text* _contentText;
    
    //Data
    string _title;
    string _content;
protected:
    NoticeBox(string title,string content);
public:
    ~NoticeBox();
    
    static NoticeBox* create(string title,string content);
    
    virtual bool init();
};

#endif /* defined(__Parallelformosa_Cocos2dx__NoticeBox__) */
