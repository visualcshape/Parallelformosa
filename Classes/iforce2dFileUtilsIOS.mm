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

using namespace std;

bool copyFile(string src, string dst)
{
    NSString *srcPath = [NSString stringWithUTF8String:src.c_str()];
    NSString *dstPath = [NSString stringWithUTF8String:dst.c_str()];
	
    NSError* error = nil;
    if ( ! [[NSFileManager defaultManager] copyItemAtPath:srcPath toPath:dstPath error:&error] ) {
        NSLog(@"Could not copy file: %@ -> %@", srcPath, dstPath);
        NSLog(@"copyItemAtPath error: %@", error);
        return false;
    }    
    return true;
}

bool createFolder(string foldername)
{
    NSString* fullpath = [NSString stringWithUTF8String:foldername.c_str()];

    NSError *error = nil;
    if ( ! [[NSFileManager defaultManager] createDirectoryAtPath:fullpath
                                   withIntermediateDirectories:YES
                                                    attributes:nil
                                                         error:&error]) {
        NSLog(@"Could not create folder: %@", fullpath);
        NSLog(@"createDirectoryAtPath error: %@", error);
        return false;
    }

    return true;
}

bool deleteFile( string filename )
{
    NSString* fullpath = [NSString stringWithUTF8String:filename.c_str()];

    NSError *error = nil;
    if ( ! [[NSFileManager defaultManager] removeItemAtPath:fullpath error:&error] ){
        NSLog(@"Could not delete file: %@", fullpath);
        NSLog(@"removeItemAtPath error: %@", error);
        return false;
    }
    
    return true;
}
