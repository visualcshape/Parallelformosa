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
}

void LoginTitleModel::setUID(std::string uid)
{
    _uid = uid;
    Notify();
}

std::string LoginTitleModel::getUID()
{
    return _uid;
}