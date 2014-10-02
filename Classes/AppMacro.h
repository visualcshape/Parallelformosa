//
//  AppMacro.h
//  Parallelformosa_Cocos2dx
//
//  Created by Chi-Chia Sun on 2014/7/27.
//
//

#ifndef Parallelformosa_Cocos2dx_AppMacro_h
#define Parallelformosa_Cocos2dx_AppMacro_h
#include <cocos2d.h>

#define DESIGN_RESOLUTION_480X320   0
#define DESIGN_RESOLUTION_960X640   1
#define DESIGN_RESOLUTION_1024X768  2
#define DESIGN_RESOLUTION_2048X1536 3

//Dialogue Window (Not in use)
#define DIALOGUE_WINDOW_START_COLOR_R 0
#define DIALOGUE_WINDOW_START_COLOR_G 0
#define DIALOGUE_WINDOW_START_COLOR_B 0
#define DIALOGUE_WINDOW_START_COLOR_A 0

#define DIALOGUE_WINDOW_END_COLOR_R 0
#define DIALOGUE_WINDOW_END_COLOR_G 0
#define DIALOGUE_WINDOW_END_COLOR_B 0
#define DIALOGUE_WINDOW_END_COLOR_A 0
//

//server config
#define GATE_HOST "192.168.2.102"
#define GATE_PORT 3150
//

//SQLite DB config
//DB_REWRITE 1 = rewrite, 0 = no-rewrite
#define DB_REWRITE 1
//

//other
#define DIALAGOUE_TAG 999999
#define DIALOGUE_TAG 999999
//No connection 0 = false 1 = true will skip connection step at title scene.
#define NO_CONNECTION 1
//

//Color define
#define COLOR_ICE_CREAM_BLUE color3B(166,209,200)

//Rect Define
#define BASE_WINDOW_INSET_RECT Rect(2,2,124,124)

#define TARGET_DESIGN_RESOLUTION_SIZE DESIGN_RESOLUTION_960X640

static std::string CONNECTOR_HOST = "";
static long long CONNECTOR_PORT = 0;

typedef struct tagResource
{
    cocos2d::Size size;
    char dir[100];
}Resource;

static Resource smallResource = {cocos2d::Size(480, 320),"Small"};
static Resource normalResource = {cocos2d::Size(960, 640),"Normal"};
//No plan to make HD version currently.
//static Resource hdResource = {cocos2d::Size(2048,1536), "Large"};

static cocos2d::Size designResolutionSize = cocos2d::Size(960, 640);

static float computeFontSize(float fontSize)
{
    return cocos2d::Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width / normalResource.size.width * fontSize;
}

#endif
