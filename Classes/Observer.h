//
//  Observer.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/31.
//
//

#ifndef Parallelformosa_Cocos2dx_Observer_h
#define Parallelformosa_Cocos2dx_Observer_h

#include "Subject.h"

class Subject;

/**@brief
 An abstract Observer class.(Observer)
 */
class Observer
{
public:
    virtual ~Observer();
    
    virtual void Update(Subject* changedSubject) = 0;
protected:
    Observer();
};

#endif
