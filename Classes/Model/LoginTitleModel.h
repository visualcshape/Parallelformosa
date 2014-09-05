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
#include <cocos2d.h>

/**@brief A Concrete Subject Login Title Model
 */
class LoginTitleModel:public Subject
{
public:
    static LoginTitleModel* getInstance();
    
    //set
    void setUID(std::string uid);
    void setBindedLayer(cocos2d::Layer* layer);
    
    //get
    std::string getUID();
    cocos2d::Layer* getBindedLayer();
protected:
    LoginTitleModel();
private:
    std::string _uid;
    static LoginTitleModel* _instance;
};


#endif
