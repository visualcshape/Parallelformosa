//
//  Database.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/30.
//
//

#include "Database.h"
#include <cocos2d.h>
#include <fstream>
#include "UtilFunc.h"
#include "AppMacro.h"

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
    
    //Copy database to writable path...
    //Android
    /**@brief Implement sooner or later*/
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //Implement later.
    std::string dbPath = FileUtils::getInstance()->fullPathForFilename("database.sqlite3");
    dbPath = FileUtils::getInstance()->getWritablePath();
    dbPath +="/database.sqlite3";

    char firstAccess[16];
        //First Access?
    FILE* firstAccessFile = fopen((FileUtils::getInstance()->getWritablePath()+"/First").c_str(), "r");
   if (firstAccessFile==nullptr)
   {
            firstAccessFile = fopen((FileUtils::getInstance()->getWritablePath()+"/First").c_str(), "w");
            fputs("1", firstAccessFile);
            fclose(firstAccessFile);
   }
   fclose(firstAccessFile);

   std::fstream fin;
   fin.open((FileUtils::getInstance()->getWritablePath()+"/First").c_str(),std::ios::in);
   fin.getline(firstAccess, sizeof(firstAccess));
   fin.close();

        //Copy resource db to document.
   if(strcmp("1", firstAccess)==0)
   {
	   ssize_t size;
	   const char* data = (char*)FileUtils::getInstance()->getFileData("database.sqlite3", "rb", &size);
       FILE* file = fopen(dbPath.c_str(), "wb");
       CC_ASSERT(file!=nullptr);
       fwrite(data, size, 1, file);
       fclose(file);
            //CC_SAFE_DELETE_ARRAY(file);
            //Re write to first Access file
#if DB_REWRITE == 1
       firstAccessFile = fopen((FileUtils::getInstance()->getWritablePath()+"First").c_str(), "w");
       strcpy(firstAccess, "1");
       fputs(firstAccess, firstAccessFile);
       fclose(firstAccessFile);
#else
       firstAccessFile = fopen((FileUtils::getInstance()->getWritablePath()+"First").c_str(), "w");
       strcpy(firstAccess, "0");
       fputs(firstAccess, firstAccessFile);
       fclose(firstAccessFile);
#endif
   }
   result = sqlite3_open(dbPath.c_str(), &_pdb);
#else   //other
    std::string path=FileUtils::getInstance()->getWritablePath()+"database.sqlite3";
    char firstAccess[16];
    //First Access?
    FILE* firstAccessFile = fopen((FileUtils::getInstance()->getWritablePath()+"First").c_str(), "r");
    if (firstAccessFile==nullptr)
    {
        firstAccessFile = fopen((FileUtils::getInstance()->getWritablePath()+"First").c_str(), "w");
        fputs("1", firstAccessFile);
        fclose(firstAccessFile);
    }
    fclose(firstAccessFile);
    
    std::fstream fin;
    fin.open((FileUtils::getInstance()->getWritablePath()+"First").c_str(),std::ios::in);
    fin.getline(firstAccess, sizeof(firstAccess));
    fin.close();
    
    //Copy resource db to document.
    if(strcmp("1", firstAccess)==0)
    {
        ssize_t size;
        const char* data = (char*)FileUtils::getInstance()->getFileData("database.sqlite3", "rb", &size);
        FILE* file = fopen(path.c_str(), "wb");
        CC_ASSERT(file!=nullptr);
        fwrite(data, size, 1, file);
        fclose(file);
        //CC_SAFE_DELETE_ARRAY(file);
        //Re write to first Access file
#if DB_REWRITE == 1
        firstAccessFile = fopen((FileUtils::getInstance()->getWritablePath()+"First").c_str(), "w");
        strcpy(firstAccess, "1");
        fputs(firstAccess, firstAccessFile);
        fclose(firstAccessFile);
#else
        firstAccessFile = fopen((FileUtils::getInstance()->getWritablePath()+"First").c_str(), "w");
        strcpy(firstAccess, "0");
        fputs(firstAccess, firstAccessFile);
        fclose(firstAccessFile);
#endif
    }
        result = sqlite3_open(path.c_str(), &_pdb);
#endif
    
    if(result!=SQLITE_OK)
    {
        CCLOG("Open database failed!, number:%d",result);
        CC_ASSERT(result==SQLITE_OK);
    }
    else
    {
        this->_insertUserID();
        CCLOG("Database initalized!");
    }
}

/*
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
    std::string select("select");
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
    else if (strcmp(pch, select.c_str())==0)
    {
        curString = select;
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
*/

/** @brief
 Insert a user ID 0 if first initialize.
 */
void Database::_insertUserID()
{
    char** resultTabel;
    char* pzErrMsg;
    int row,column,result;
    
    //Count rows..
    result = sqlite3_get_table(_pdb, "select count(*) from User", &resultTabel, &row, &column, &pzErrMsg);
    CCASSERT(result==SQLITE_OK,pzErrMsg);
    if(strcmp(resultTabel[1], "0")==0)
    {
        //Insert new ID
        result = sqlite3_exec(_pdb, "insert into User values(0,1,0,0,0,'0.0.0',0,0,0,0,0,0,0)", nullptr, nullptr, nullptr);
        
        CCASSERT(result==SQLITE_OK, "_insertUserID new ID Failed!");
    }
    //if already exist do nothing.
    CCLOG("Database : _insertUserID sucess!");
    sqlite3_free_table(resultTabel);
    
}

sqlite3* Database::getDatabasePointer()
{
    return _pdb;
}
