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
#include <dirent.h>
#include <string>
#include <vector>

// Functions here are the same for both iOS and Android

using namespace std;

int listFolder(string folder, vector<string>& entries, bool directoriesOnly)
{
    DIR* dp = opendir(folder.c_str());
    if (dp == NULL) {
        return entries.size();
    }

    struct dirent *entry = NULL;
    while((entry = readdir(dp))) {
        if ( directoriesOnly && entry->d_type != DT_DIR )
            continue;
        entries.push_back( entry->d_name );
    }
    closedir(dp);

    return entries.size();
}
