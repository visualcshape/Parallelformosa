//
//  Database.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/30.
//
//

#ifndef Parallelformosa_Cocos2dx_Database_h
#define Parallelformosa_Cocos2dx_Database_h

#include <sqlite3.h>
#include <string>

/** @brief A singleton database
 Please using 'getInstance' to access.
 */
class Database
{
public:
    static Database* getInstance();
    void initDatabase();
    //void sqlQuery(std::string query); //Stop using this temporary.
    sqlite3* getDatabasePointer();
protected:
    Database();
private:
    void _insertUserID();
    static Database* _instance;
    sqlite3* _pdb;
};


#endif