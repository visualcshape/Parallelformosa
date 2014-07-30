//
//  UtilFunc.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/30.
//
//

#include "UtilFunc.h"
#include <ctype.h>

UtilFunc* UtilFunc::_instance = nullptr;

UtilFunc::UtilFunc()
{
    
}

/** @brief Return an instance.
 */
UtilFunc* UtilFunc::getInstance()
{
    //Lazy init.
    if(_instance==nullptr)
    {
        _instance = new UtilFunc();
    }
    
    return _instance;
}

/** @brief Return a string that all lower.
 */
void UtilFunc::toLower(std::string* str)
{
    for (int i = 0; i<str->length(); i++) {
        (*str)[i] = tolower(str->c_str()[i]);
    }
}