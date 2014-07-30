//
//  Database.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/30.
//
//

#include "Database.h"
#include <cocos2d.h>
#include "UtilFunc.h"

USING_NS_CC;

Database* Database::_instance = nullptr;

Database::Database()
{
    
}

Database* Database::getInstance()
{
    if(_instance==nullptr)
    {
        _instance = new Database();
    }
    return _instance;
}

void Database::initDatabase()
{
    int result;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    std::string dbPath;
    dbPath = FileUtils::getInstance()->getWritablePath();
    dbPath += "/database.sqlite3";
    
    FILE* file = fopen(dbPath.c_str(), "r");
    if(file==nullptr)
    {
        CC_ASSERT(file!=nullptr);
    }
    fclose(file);
    result = sqlite3_open(dbPath.c_str(), &_pdb);
#else
    std::string path=FileUtils::getInstance()->getWritablePath()+"database.sqlite3";
    result = sqlite3_open(path.c_str(), &_pdb);
#endif
    
    if(result!=SQLITE_OK)
    {
        CCLOG("Open database failed!, number:%d",result);
        CC_ASSERT(result==SQLITE_OK);
    }
    else
    {
        CCLOG("Database initalized!");
    }
}

void Database::sqlQuery(std::string query)
{
    //tok
    char* str;
    const char* delim = " ";
    char* pch;
    //
    //Query types
    std::string curString("");
    std::string insertion("insert");
    std::string del("delete");
    //
    //Get type
    std::string tmpStr("");
    strcpy(str, query.c_str());
    pch = std::strtok(str, delim);
    tmpStr = pch;
    UtilFunc::getInstance()->toLower(&tmpStr);
    strcpy(pch, tmpStr.c_str());
    //Set current type
    if (strcmp(pch, insertion.c_str())==0) {
        curString = insertion;
    }
    else if(strcmp(pch, del.c_str())==0)
    {
        curString = del;
    }
    //No such type
    else
    {
        curString = "-1";
    }
    CCASSERT(strcmp(curString.c_str(), "-1")!=0, "sqlQuery : No such type!");
    //
    
    int result = sqlite3_exec(_pdb, query.c_str(), nullptr, nullptr, nullptr);
    if (result!=SQLITE_OK) {
        char* err;
        strcat(err, (curString+" data failed!").c_str());
        CCLOG("%s",err);
    }
}