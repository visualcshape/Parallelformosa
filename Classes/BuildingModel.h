//
//  BuildingModel.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/5.
//
//

#ifndef __Parallelformosa_Cocos2dx__BuildingModel__
#define __Parallelformosa_Cocos2dx__BuildingModel__

#include "Subject.h"
#include <cocos2d.h>

USING_NS_CC;
using namespace cocos2d::ui;

class BuildingModel:public Subject{
private:
    static BuildingModel* _instance;
protected:
    BuildingModel();
public:
    ~BuildingModel();
    
    static BuildingModel* getInstance();
};

#endif /* defined(__Parallelformosa_Cocos2dx__BuildingModel__) */
