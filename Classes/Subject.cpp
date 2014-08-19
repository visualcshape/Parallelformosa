//
//  Subject.cpp
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/31.
//
//

#include "Subject.h"

Subject::Subject()
{
    _observers = new std::list<Observer*>;
}

Subject::~Subject()
{
    delete _observers;
}

void Subject::Attach(Observer *observer)
{
    _observers->push_back(observer);
}

void Subject::Detach(Observer *observer)
{
    _observers->remove(observer);
}

void Subject::Notify()
{
    std::list<Observer*>::iterator it;
    
    //Fix
    for(it = _observers->begin() ; it!=_observers->end();it++)
    {
        Observer* cur = *it;
        cur->Update(this);
    }
}

std::string Subject::getTypeName()
{
    return _typeName;
}
