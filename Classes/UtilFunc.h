//
//  UtilFunc.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/30.
//
//

#ifndef Parallelformosa_Cocos2dx_UtilFunc_h
#define Parallelformosa_Cocos2dx_UtilFunc_h

#include <string>

/** @brief A Singleton function
 Utils function like to lower and etc..
 */
class UtilFunc
{
private:
    static UtilFunc* _instance;
protected:
    UtilFunc();
public:
    static UtilFunc* getInstance();
    void toLower(std::string* str);
};



#endif
