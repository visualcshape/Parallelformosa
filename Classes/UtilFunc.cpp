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

void UtilFunc::copyPlayerMapTMXFile(string coord)
{
    if(checkWritablePathFileExist(coord+".tmx"))
    {
        //No nned to copy another
        return;
    }
    else
    {
#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID
        FILE* fpReadPlayerBase = fopen((FileUtils::getInstance()->getWritablePath()+"/"+"playerMap.tmx").c_str(), "rb");
        FILE* fpReadPlayerBase = fopen((FileUtils::getInstance()->getWritablePath()+"/"+coord+".tmx").c_str(), "wb");
#else
        FILE* fpReadPlayerBase = fopen((FileUtils::getInstance()->getWritablePath()+"playerMap.tmx").c_str(), "rb");
        FILE* fpWritePlayerOwnMap = fopen((FileUtils::getInstance()->getWritablePath()+coord+".tmx").c_str(), "wb");
#endif
        CC_ASSERT(fpReadPlayerBase!=nullptr);
        CC_ASSERT(fpWritePlayerOwnMap!=nullptr);
        
        unsigned char* buffer = nullptr;
        ssize_t size = 0;
        fseek(fpReadPlayerBase, 0, SEEK_END);
        size = ftell(fpReadPlayerBase);
        fseek(fpReadPlayerBase, 0, SEEK_SET);
        buffer = (unsigned char*)malloc(size);
        size = fread(buffer, sizeof(unsigned char), size, fpReadPlayerBase);
        fwrite(buffer, size, 1, fpWritePlayerOwnMap);
        fclose(fpWritePlayerOwnMap);
        fclose(fpReadPlayerBase);
        //!
        free(buffer);
        //
    }
}

bool UtilFunc::checkWritablePathFileExist(string fileName)
{
    string path = FileUtils::getInstance()->getWritablePath();
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    path+="/";
#endif
    
    path+=fileName;
    
    if(FILE* file = fopen(path.c_str(), "r"))
    {
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}

void UtilFunc::downloadInfoFile(string coord,function<void(HttpClient* ,HttpResponse*)> onHttpRequestCompleteCallback)
{
    CC_ASSERT(!coord.empty());
    
    HttpRequest* req = new HttpRequest();
    
    string path = DOWNLOAD_BASE_PATH + coord + ".info";
    req->setRequestType(HttpRequest::Type::GET);
    req->setUrl(path.c_str());
    req->setTag("Downloading Info");
    req->setResponseCallback(onHttpRequestCompleteCallback);
    HttpClient::getInstance()->send(req);
    req->release();
}
