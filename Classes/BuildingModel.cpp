//
//  BuildingModel.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/5.
//
//

#include "BuildingModel.h"

BuildingModel* BuildingModel::_instance=nullptr;

bool BuildingModel::_isInit = false;

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

void BuildingModel::initModelAsync(std::function<void()> loadedCompleteCallback)
{
    std::thread thread(&BuildingModel::initModel,this);
    thread.join();
    //call callback
    loadedCompleteCallback();
}

void BuildingModel::initModel()
{
    //sqlite3
    sqlite3* pDB = Database::getInstance()->getDatabasePointer();
    int result;
    char* errMsg;
    
    std::string sql = "select * from BuildingType";
    result = sqlite3_exec(pDB, sql.c_str(), BuildingModel::queryResult, nullptr, &errMsg);
    CCASSERT(result==SQLITE_OK, errMsg);
    
    return;
}

int BuildingModel::queryResult(void *para, int columns, char **columnValue, char **columnName)
{
    BuildingType tmp;
    //
    for(int i = 0 ; i < columns ; i++)
    {
        std::string columnValueStr = columnValue[i];
        std::string columnNameStr = columnName[i];
        
        if(columnNameStr=="Name")
        {
            tmp.name = columnValueStr;
        }
        else if(columnNameStr=="ResourceThumb")
        {
            tmp.thumbResourceName = columnValueStr;
        }
        else if(columnNameStr=="BaseHP")
        {
            tmp.baseHP = atoi(columnValueStr.c_str());
        }
        else if(columnNameStr=="Type")
        {
            tmp.type = columnValueStr;
        }
        else if(columnNameStr=="Range")
        {
            tmp.range = atoi(columnValueStr.c_str());
        }
        else if(columnNameStr=="FoodCost")
        {
            tmp.foodCost = atoi(columnValueStr.c_str());
        }
        else if(columnNameStr=="GPowerCost")
        {
            tmp.gPower = atoi(columnValueStr.c_str());
        }
        else if(columnNameStr=="LManaCost")
        {
            tmp.lMana = atoi(columnValueStr.c_str());
        }
        else if(columnNameStr=="LevelFactorCost")
        {
            tmp.levelFactorCost = atof(columnValueStr.c_str());
        }
        else if(columnNameStr=="LevelFactorHP")
        {
            tmp.levelFactorHP = atof(columnValueStr.c_str());
        }
        else if(columnNameStr=="OcuppyWidth")
        {
            tmp.width = atoi(columnValueStr.c_str());
        }
        else if(columnNameStr=="OcuppyHeight")
        {
            tmp.height = atoi(columnValueStr.c_str());
        }
        else if(columnNameStr=="Description")
        {
            tmp.descrption = columnValueStr;
        }
        else if(columnNameStr=="Duration")
        {
            tmp.duration = atoi(columnValueStr.c_str());
        }
        else if(columnNameStr=="DurationFactor")
        {
            tmp.durationFactor = atof(columnValueStr.c_str());
        }
        else
        {
            CCASSERT(false, "[DB]Error no such type.");
        }
    }
    //push to map
    BuildingModel::getInstance()->insertIntoMap(tmp.name,tmp);
    
    return SQLITE_OK;
}

void BuildingModel::insertIntoMap(std::string key, BuildingType type)
{
    _buildingTypesMap[key] = type;
}

const std::map<std::string,BuildingType>* BuildingModel::getBuildingModelMap()
{
    return &_buildingTypesMap;
}