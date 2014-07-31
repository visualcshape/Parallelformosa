//
//  LoginTitleModel.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/31.
//
//

#ifndef Parallelformosa_Cocos2dx_LoginTitleModel_h
#define Parallelformosa_Cocos2dx_LoginTitleModel_h

#include "Subject.h"
#include <string>

/**@brief A Concrete Subject Login Title Model
 */
class LoginTitleModel:public Subject
{
public:
    static LoginTitleModel* getInstance();
    
    void setUID(std::string uid);
    std::string getUID();
protected:
    LoginTitleModel();
private:
    std::string _uid;
    static LoginTitleModel* _instance;
};


#endif
