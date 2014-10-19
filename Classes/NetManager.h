//
//  NetManager.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/18.
//
//

#ifndef __Parallelformosa_Cocos2dx__NetManager__
#define __Parallelformosa_Cocos2dx__NetManager__

#include "CCPomeloWrapper.h"
#include <cocos2d.h>
#include <CocosGUI.h>
#include "DialogueWindowConfirm.h"

using namespace std;
USING_NS_CC;
USING_NS_CC::ui;

class NetManager
{
private:
    static NetManager* _instance;
    static string DIALOGUE_NAME;
protected:
    NetManager();
public:
    ~NetManager();
    static NetManager* getInstance();
    
    void onConnectLost();
    
    void OKCB(Ref* pSender,Widget::TouchEventType type);
};

#endif /* defined(__Parallelformosa_Cocos2dx__NetManager__) */
