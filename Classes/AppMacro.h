#pragma once
#include <cocos2d.h>
#include <vector>
#include <algorithm>
#include <bitset>

USING_NS_CC;
using namespace std;

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
#define GATE_HOST "169.254."
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

//BaseWindow(WindowProtocol) Width

#define TARGET_DESIGN_RESOLUTION_SIZE DESIGN_RESOLUTION_960X640

#define TEST_TEXT "testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest"

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

// Personal Macro {
#define SZ(a) ((int)(a).size())
typedef std::pair <int, int> PII;
typedef Vec2 TilePoint;
typedef Vec2 MapPoint;
#define X first
#define Y second
#define MP std::make_pair
#define FOR(it, c) for( decltype((c).begin()) it = (c).begin(); it != (c).end(); it++)
enum HUD_ID{ EMPTY = 0, ATTACK = 1, DEFENSE = 2 };
enum { LEFT = 0, DOWN = 1, RIGHT = 2, UP = 3 , NO = 4};
static const int OFFX[] = { -1, 0, 1, 0 };
static const int OFFY[] = { 0, -1, 0, 1 };
#define MAP_MAX_SIZE 35
#define MAP_X_MAX 25
#define MAP_Y_MAX 16
#define INF 1023456789

#define EXECUTE_RATE 1.0f
// }
