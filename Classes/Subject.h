//
//  Subject.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/31.
//
//

#ifndef Parallelformosa_Cocos2dx_Subject_h
#define Parallelformosa_Cocos2dx_Subject_h

#include "Observer.h"
#include <string>
#include <list>

/**@brief
 Subject class : Observer Model.
 */

class Observer;

class Subject
{
public:
    virtual ~Subject();
    
    virtual void Attach(Observer* observer);
    virtual void Detach(Observer* observer);
    virtual void Notify();
    virtual std::string getTypeName();
protected:
    Subject();
    std::string _typeName;
private:
    std::list<Observer*>* _observers;
};


#endif
