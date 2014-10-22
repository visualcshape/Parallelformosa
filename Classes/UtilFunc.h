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
#include <vector>
#include <sstream>
#include <cocos2d.h>
#include <HttpClient.h>
#include "AppMacro.h"

USING_NS_CC;
USING_NS_CC::network;

using namespace std;
/** @brief A Singleton function
Utils function like to lower and etc..
*/
class UtilFunc
{
private:
    static UtilFunc* _instance;
    
    std::vector<std::string> &split(const std::string &s,char delim,std::vector<std::string>& elems);
protected:
	UtilFunc();
public:
	/** @brief Return an instance.
	*/
	static UtilFunc* getInstance();

	/** @brief Return a string that all lower.
	*@param String to lower.
	*/
	void toLower(std::string* str);

	std::vector<std::string> split(const std::string&s, char delim);

	void copyPlayerMapTMXFile(string coord);

	bool checkWritablePathFileExist(string fileName);

	void downloadInfoFile(string coord, function<void(HttpClient*, HttpResponse*)> onHttpRequestCompleteCallback);
};



#endif
