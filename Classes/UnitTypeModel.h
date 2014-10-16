//
//  UnitTypeModel.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/12.
//
//

#ifndef __Parallelformosa_Cocos2dx__UnitTypeModel__
#define __Parallelformosa_Cocos2dx__UnitTypeModel__

#include <map>
#include <cocos2d.h>
#include "Database.h"
#include <thread>

USING_NS_CC;
using namespace std;

//No more files for types
typedef struct UnitType
{
    string name;
    string resourceThumb;
    int baseHP;
    float levelFactorHP;
    string type;
    int range;
    int foodCost;
    int gPowerCost;
    int lManaCost;
    int duration;
    float durationFactor;
    float levelFactorCost;
    string description;
    int atk;
    int def;
    int id;
    int gid;
    string resourcePath;
}UnitType;


class UnitTypeModel
{
private:
    static UnitTypeModel* _instance;
    
    map<string,UnitType> _unitTypeMap;
protected:
    UnitTypeModel();
    
    static int queryResultCallback(void* para,int columns,char** columnValue,char** columnName);
public:
    ~UnitTypeModel();
    
    static UnitTypeModel* getInstance();
    
    void initModelAsync(function<void()> loadedCompleteCallback);
    
    void initModel();
    
    void insertIntoMap(string key,UnitType type);
    
    const map<string,UnitType>* getUnitTypeMap();
};

#endif /* defined(__Parallelformosa_Cocos2dx__UnitTypeModel__) */
