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

UtilFunc* UtilFunc::getInstance()
{
    //Lazy init.
    if(_instance==nullptr)
    {
        _instance = new UtilFunc();
    }
    
    return _instance;
}

void UtilFunc::toLower(std::string* str)
{
    for (int i = 0; i<str->length(); i++) {
        (*str)[i] = tolower(str->c_str()[i]);
    }
}

std::vector<std::string> &UtilFunc::split(const std::string &s, char delim, std::vector<std::string>& elems)
{
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    
    return elems;
}

std::vector<std::string> UtilFunc::split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s,delim,elems);
    return elems;
}