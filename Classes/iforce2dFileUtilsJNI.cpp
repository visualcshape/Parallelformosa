/*
* Author: Chris Campbell - www.iforce2d.net
*
* See usage details at http://www.iforce2d.net/cc2dxres/fileutils
*
* This software is provided 'as-is', without any express or implied warranty.
* In no event will the authors be held liable for any damages arising from
* the use of this software. Permission is granted to anyone to use this software
* for any purpose, including commercial applications, and to alter it and
* redistribute it freely.
*
*/
#include <string>
#include <cocos2d.h>
#include "platform/android/jni/JniHelper.h"

#define  CLASS_NAME "org/iforce2d/otfdemo/otfdemo"

using namespace std;
using namespace cocos2d;

bool copyFile(string src, string dst)
{
    JniMethodInfo minfo;
    bool ok = JniHelper::getStaticMethodInfo(minfo, CLASS_NAME, "copyFile", "(Ljava/lang/String;Ljava/lang/String;)Z");
    if ( !ok ) {
        CCLOG("Could not find instance method copyFile");
        return false;
    }
    jstring jstr1 = minfo.env->NewStringUTF( src.c_str() );
    jstring jstr2 = minfo.env->NewStringUTF( dst.c_str() );
    minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID, jstr1, jstr2);
    return true;
}

bool createFolder(string foldername)
{
    JniMethodInfo minfo;
    bool ok = JniHelper::getStaticMethodInfo(minfo, CLASS_NAME, "createFolder", "(Ljava/lang/String;)Z");
    if ( !ok ) {
        CCLOG("Could not find instance method createFolder");
        return false;
    }
    jstring jstr = minfo.env->NewStringUTF( foldername.c_str() );
    minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID, jstr);
    return true;
}

bool deleteFile( string filename )
{
    JniMethodInfo minfo;
    bool ok = JniHelper::getStaticMethodInfo(minfo, CLASS_NAME, "deleteFile2", "(Ljava/lang/String;)Z");
    if ( !ok ) {
        CCLOG("Could not find instance method deleteFile2");
        return false;
    }
    jstring jstr = minfo.env->NewStringUTF( filename.c_str() );
    minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID, jstr);
    return true;
}
