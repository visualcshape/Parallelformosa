//
//  MainUIInfoModel.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/1.
//
//

#ifndef __Parallelformosa_Cocos2dx__MainUIInfoModel__
#define __Parallelformosa_Cocos2dx__MainUIInfoModel__

#include "Subject.h"

using namespace std;

class MainUIInfoModel : public Subject{
public:
    enum ChangedData{
        NONE = -1,
        SCROLLING_TEXT
    };
private:
    string _scorllingLabelText;
    ChangedData _changedData;
protected:
    MainUIInfoModel();
public:
    ~MainUIInfoModel();
    
    static MainUIInfoModel* _instance;
    
    static MainUIInfoModel* getInstance();
    
    string getScrollingText();
    
    void setScrollingText(string text);
    
    ChangedData getChagedData();
};

#endif /* defined(__Parallelformosa_Cocos2dx__MainUIInfoModel__) */
