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
    
    char** resultTabel;
    char* pErrMsg;
    int row,column,result;
    std::vector<std::string> attributes;
    std::map<std::string,int> order;
    
    std::string sql = "select * from BuildingType";
    result = sqlite3_get_table(pDB, sql.c_str(), &resultTabel, &row, &column, &pErrMsg);
    CCASSERT(result==SQLITE_OK, pErrMsg);
    
    //order up
    for(int i = 0 ; i < column ; i++)
    {
        order[resultTabel[i]] = i;
    }
    
    //push back to
    
    for(int i = column,j = 0 ; i < row ; i++,j++)
    {
        
        
        if(j==column-1)
            j=-1;
    }
}