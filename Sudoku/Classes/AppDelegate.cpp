#include "AppDelegate.h"

#include <vector>
#include <string>

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include "ResolutionAdaptor.h"
#include "LoadingScene.h"

#include "AppMacros.h"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

    CCSize frameSize = pEGLView->getFrameSize();
    
    vector<string> searchPath;
    XAXA::ResolutionAdaptor resAdapter(frameSize.width,frameSize.height);
    int x,y;
    resAdapter.exec(x,y,searchPath);

    CCLOG("frame size: %f,%f design size: %d,%d", frameSize.width, frameSize.height, x, y);
    pEGLView->setDesignResolutionSize(x,y,kResolutionShowAll);

    // set searching path
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);

    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = XAXA::LoadingScene::scene();
    //REPLACE_SCENE_FROM_CCBI(MainLayer);
    //RUN_SCENE_FROM_CCBI(MainLayer);
    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
