#include "AppDelegate.h"
#include "AppMacro.h"
#include "Database.h"
#include "UtilFunc.h"
#include "SceneManager.h"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Parallelformosa");
        director->setOpenGLView(glview);
    }
    
    //**
    //WP8 CROSS PLATFORM NOT SUPPORTED
    //**
    
    //Set design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
    
    Size frameSize = glview->getFrameSize();
    
    vector<string> searchPath;
    
    if (frameSize.height > smallResource.size.height)
    {
        searchPath.push_back(normalResource.dir);
        director->setContentScaleFactor(MIN(normalResource.size.height/designResolutionSize.height, normalResource.size.width/designResolutionSize.width));
    }
    else
    {
        searchPath.push_back(smallResource.dir);
        director->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }
    
    //set searching path
    FileUtils::getInstance()->setSearchPaths(searchPath);
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    //Reminder : to put scene
    
    //Initialize database
    Database::getInstance()->initDatabase();
    
    //run
	SceneManager::goMapScreen();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
