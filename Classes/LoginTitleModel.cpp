//
//  LoginTitleModel.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/31.
//
//

#include "LoginTitleModel.h"
#include <sqlite3.h>
#include <cocos2d.h>
#include "Database.h"

LoginTitleModel* LoginTitleModel::_instance = nullptr;

LoginTitleModel* LoginTitleModel::getInstance()
{
    if(_instance==nullptr)
    {
        _instance = new LoginTitleModel();
    }
    return _instance;
}

LoginTitleModel::LoginTitleModel()
{
    char** table;
    char* pErrMsg;
    int column,row;
    
    int result = sqlite3_get_table(Database::getInstance()->getDatabasePointer(), "select ID from User", &table, &row, &column, &pErrMsg);
    CCASSERT(result==SQLITE_OK, pErrMsg);
    
    _uid = table[1];
    
    _typeName = "LoginTitleModel";
}

void LoginTitleModel::setUID(std::string uid)
{
    char* pErrMsg;
    std::string originalUID = _uid;
    std::string sql("");
    
    //make query
    sql = "update User set ID="+uid+" where ID="+originalUID;
    //
    
    _uid = uid;
    
    int result = sqlite3_exec(Database::getInstance()->getDatabasePointer(), sql.c_str(), nullptr, nullptr, &pErrMsg);
    CCASSERT(result==SQLITE_OK, pErrMsg);
    //
    
    Notify();
}

std::string LoginTitleModel::getUID()
{
    return _uid;
}