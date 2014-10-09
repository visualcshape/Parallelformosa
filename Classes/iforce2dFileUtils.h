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
#ifndef IFORCE2D_FILEUTILS_H
#define IFORCE2D_FILEUTILS_H

#include <string>
#include <vector>

bool copyFile(std::string src, std::string dst);
bool deleteFile(std::string file);
bool createFolder(std::string folder);
int listFolder(std::string folder, std::vector< std::string >& entries, bool directoriesOnly);

#endif // IFORCE2D_FILEUTILS_H
