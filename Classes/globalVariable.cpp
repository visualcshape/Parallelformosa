//
//  globalVariable.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/16.
//
//

#include "globalVariable.h"

GlobalVariable* GlobalVariable::_instance = nullptr;

GlobalVariable* GlobalVariable::getInstance()
{
    if(_instance==nullptr)
    {
        _instance = new GlobalVariable();
        return _instance;
    }
    return _instance;
}

GlobalVariable::GlobalVariable()
{
    
}

GlobalVariable::~GlobalVariable()
{
    
}

void GlobalVariable::setConnectorHost(string host)
{
    _connectorHost = host;
}

string GlobalVariable::getConnectorHost()
{
    return _connectorHost;
}

void GlobalVariable::setConnectorPort(int port)
{
    _connectorPort = port;
}

int GlobalVariable::getConnectorPort()
{
    return _connectorPort;
}