//
//  globalVariable.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/16.
//
//

#ifndef __Parallelformosa_Cocos2dx__globalVariable__
#define __Parallelformosa_Cocos2dx__globalVariable__

#include <string>

using namespace std;

class GlobalVariable
{
private:
    int _connectorPort;
    string _connectorHost;
    
    static GlobalVariable* _instance;
    
protected:
    GlobalVariable();
public:
    static GlobalVariable* getInstance();
    
    void setConnectorPort(int port);
    int getConnectorPort();
    
    void setConnectorHost(string host);
    string getConnectorHost();
    
    ~GlobalVariable();
};

#endif /* defined(__Parallelformosa_Cocos2dx__globalVariable__) */
