//
//  BuildingType.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/10/7.
//
//

#ifndef __Parallelformosa_Cocos2dx__BuildingType__
#define __Parallelformosa_Cocos2dx__BuildingType__

#include <string>

typedef struct BuildingType{
    std::string name;
    std::string thumbResourceName;
    int baseHP;
    std::string type;
    int range;
    int foodCost;
    int lMana;
    int gPower;
    float levelFactorHP;
    float levelFactorCost;
    int width;
    int height;
    std::string descrption;
    int duration;
    float durationFactor;
    int bid;
}BuildingType;

#endif /* defined(__Parallelformosa_Cocos2dx__BuildingType__) */
