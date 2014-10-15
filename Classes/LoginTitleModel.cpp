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
    char* pErrMsg;
    sqlite3* pDB = Database::getInstance()->getDatabasePointer();
    
    int result = sqlite3_exec(pDB, "select * from User", LoginTitleModel::_getUIDCallback, this, &pErrMsg);
    CCASSERT(result==SQLITE_OK, pErrMsg);
    
    _typeName = "LoginTitleModel";
}

int LoginTitleModel::_getUIDCallback(void *para, int columns, char **columnValue, char **columnName)
{
    LoginTitleModel* modelPointer = static_cast<LoginTitleModel*>(para);
    for(int i = 0 ; i < columns ; i++)
    {
        if(std::string(columnName[i])=="ID")
        {
            modelPointer->_uid = columnValue[i];
        }
    }
    return SQLITE_OK;
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
