//
//  UnitTypeModel.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/12.
//
//

#include "UnitTypeModel.h"


UnitTypeModel* UnitTypeModel::_instance = nullptr;

UnitTypeModel::UnitTypeModel()
{
    
}

UnitTypeModel* UnitTypeModel::getInstance()
{
    if(_instance==nullptr)
    {
        _instance = new UnitTypeModel();
        return _instance;
    }
    return _instance;
}

UnitTypeModel::~UnitTypeModel()
{
    if(_instance!=nullptr)
    {
        CC_SAFE_DELETE(_instance);
    }
}

void UnitTypeModel::initModelAsync(function<void()> loadedCompleteCallback)
{
    thread th(&UnitTypeModel::initModel,this);
    th.join();
    
    loadedCompleteCallback();
}

void UnitTypeModel::initModel()
{
    sqlite3* pDB = Database::getInstance()->getDatabasePointer();
    int result;
    char* errMsg;
    
    string sql = "select * from UnitType";
    result  =sqlite3_exec(pDB, sql.c_str(), UnitTypeModel::queryResultCallback, nullptr, &errMsg);
    CCASSERT(result==SQLITE_OK, errMsg);
    
}

int UnitTypeModel::queryResultCallback(void *para, int columns, char **columnValue, char **columnName)
{
    UnitType tmp;
    
    for(int i = 0 ; i < columns ; i++)
    {
        string columnValueStr = columnValue[i];
        string columnNameStr = columnName[i];
        if(columnNameStr=="Name")
        {
            tmp.name = columnValueStr;
        }
        else if(columnNameStr=="ResourceThumb")
        {
            tmp.resourceThumb = columnValueStr;
        }
        else if(columnNameStr=="BaseHP")
        {
            tmp.baseHP = atoi(columnValueStr.c_str());
        }
        else if(columnNameStr=="LevelFactorHP")
        {
            tmp.levelFactorHP = atof(columnValueStr.c_str());
        }
        else if (columnNameStr=="Type")
        {
            tmp.type = columnValueStr;
        }
        else if(columnNameStr=="Range")
        {
            tmp.range = atoi(columnValueStr.c_str());
        }
        else if (columnNameStr=="FoodCost")
        {
            tmp.foodCost = atoi(columnValueStr.c_str());
        }
        else if(columnNameStr=="GPowerCost")
        {
            tmp.gPowerCost = atoi(columnValueStr.c_str());
        }
        else if(columnNameStr=="LManaCost")
        {
            tmp.lManaCost = atoi(columnValueStr.c_str());
        }
        else if (columnNameStr=="Duration")
        {
            tmp.duration = atoi(columnValueStr.c_str());
        }
        else if(columnNameStr=="DurationFactor")
        {
            tmp.durationFactor = atof(columnValueStr.c_str());
        }
        else if(columnNameStr=="LevelFactorCost")
        {
            tmp.levelFactorCost = atof(columnValueStr.c_str());
        }
        else if(columnNameStr=="Description")
        {
            tmp.description = columnValueStr;
        }
        else if(columnNameStr=="ATK")
        {
            tmp.atk = atoi(columnValueStr.c_str());
        }
        else if(columnNameStr=="DEF")
        {
            tmp.def = atoi(columnValueStr.c_str());
        }
        else
        {
            CC_ASSERT(false);
        }
    }
    
    UnitTypeModel::getInstance()->insertIntoMap(tmp.name, tmp);
    
    return SQLITE_OK;
}

void UnitTypeModel::insertIntoMap(string key, UnitType type)
{
    _unitTypeMap[key] = type;
}

const map<string,UnitType>* UnitTypeModel::getUnitTypeMap()
{
    return &_unitTypeMap;
}