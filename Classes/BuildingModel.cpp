//
//  BuildingModel.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/5.
//
//

#include "BuildingModel.h"

BuildingModel* BuildingModel::_instance=nullptr;

BuildingModel::BuildingModel(){
    
}

BuildingModel::~BuildingModel(){
    if(_instance!=nullptr){
        CC_SAFE_DELETE(_instance);
    }
}

BuildingModel* BuildingModel::getInstance(){
    if(_instance==nullptr){
        _instance = new BuildingModel;
        return _instance;
    }
    return _instance;
}
