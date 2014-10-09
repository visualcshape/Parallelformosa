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
#include "Database.h"
#include "BuildingType.h"
#include <thread>
#include <map>

USING_NS_CC;
using namespace cocos2d::ui;

class BuildingModel{
private:
    static BuildingModel* _instance;
    
    static bool _isInit;
    
    BuildingType _buildingTypes;
    
    void initModel();
    
    std::map<std::string, BuildingType> _buildingTypesMap;
    
    static int queryResult(void* para,int columns,char** columnValue,char** columnName);
protected:
    BuildingModel();
    
public:
    ~BuildingModel();
    
    static BuildingModel* getInstance();
    
    void initModelAsync(std::function<void()> loadedCompleteCallback);
    
    void insertIntoMap(std::string key,BuildingType type);
    
    const std::map<std::string,BuildingType>* getBuildingModelMap();
};

#endif /* defined(__Parallelformosa_Cocos2dx__BuildingModel__) */
