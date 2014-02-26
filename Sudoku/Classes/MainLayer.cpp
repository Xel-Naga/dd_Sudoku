#include "LevelMgr.h"
#include "MainLayer.h"
#include "Resource.h"

#include "WapsAd.h"

#include "LevelLayerLoader.h"
USING_NS_CC_EXT;

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

namespace XAXA {

void MainLayer::onNewGame(CCObject* pSender) {
    /* Create an autorelease CCNodeLoaderLibrary. */
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    ccNodeLoaderLibrary->registerCCNodeLoader("LevelLayer", LevelLayerLoader::loader());

    /* Create an autorelease CCBReader. */
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    
    //非常重要，这里ccb根目录设置不正确会导致图片加载失败，找不到对应的文件
    ccbReader->setCCBRootPath("");
    /* Read a ccbi file. */
    CCNode * node = ccbReader->readNodeGraphFromFile("MainScene.ccbi", this);
    
    ccbReader->release();

    CCScene * scene = CCScene::create();
    if(node != NULL) {
        scene->addChild(node);
    }

    CCDirector::sharedDirector()->replaceScene(scene);
    CCLOG("MainLayer:: reference:%d",this->retainCount());
}

void MainLayer::onSettings(CCObject* pSender) {
    WapsAd::showAd(0);
}

void MainLayer::onAbout(CCObject* pSender) {
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MainLayer::initSound() {

    // preload background music and effect
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic( MUSIC_FILE );
    SimpleAudioEngine::sharedEngine()->preloadEffect(CLICK_FILE);
    
    // set default volume
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5f);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.3f);
    if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_FILE, true);
    }

}

void MainLayer::initLevel() {
    //关卡初始化
    //XAXA::LevelLayer* curr_level = XAXA::LevelLayer::create(); 
}

bool MainLayer::init() {
    if(!CCLayer::init()) {
        return false;
    }

    initSound();

    initLevel();

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    //CCTexture2D* pTexture = CCTexture2D::init();
    
    CCSprite* newGameNormal = CCSprite::create(s_menu, CCRect(0, 0, 126, 33));
    CCSprite* newGameSelected = CCSprite::create(s_menu, CCRect(0, 33, 126, 33));
    CCSprite* newGameDisabled = CCSprite::create(s_menu, CCRect(0, 33 * 2, 126, 33));
    
    CCSprite* gameSettingsNormal = CCSprite::create(s_menu, CCRect(126, 0, 126, 33));
    CCSprite* gameSettingsSelected = CCSprite::create(s_menu, CCRect(126, 33, 126, 33));
    CCSprite* gameSettingsDisabled = CCSprite::create(s_menu, CCRect(126, 33 * 2, 126, 33));

    CCSprite* aboutNormal = CCSprite::create(s_menu, CCRect(252, 0, 126, 33));
    CCSprite* aboutSelected = CCSprite::create(s_menu, CCRect(252, 33, 126, 33));
    CCSprite* aboutDisabled = CCSprite::create(s_menu, CCRect(252, 33 * 2, 126, 33));

    CCMenuItemSprite* newGame = CCMenuItemSprite::create(newGameNormal, newGameSelected, newGameDisabled, this, menu_selector(MainLayer::onNewGame));
    CCMenuItemSprite* gameSettings = CCMenuItemSprite::create(gameSettingsNormal, gameSettingsSelected, gameSettingsDisabled, this,menu_selector(MainLayer::onSettings));
    CCMenuItemSprite* about = CCMenuItemSprite::create(aboutNormal, aboutSelected, aboutDisabled, this, menu_selector(MainLayer::onAbout));

    //非常重要，菜单最后一个对象必须是NULL
    CCMenu* menu = CCMenu::create(newGame, gameSettings, about,NULL);

    menu->alignItemsVerticallyWithPadding(10);

    CCSize s = CCDirector::sharedDirector()->getWinSize();
    menu->setPosition(ccp(s.width/2, s.height/2));

    this->addChild(menu, 1);

    //默认添加进来是sprite的中心位置在坐标原点
    CCSprite* mainBkg = CCSprite::create(s_MainBkg);
    mainBkg->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(mainBkg);
    return true;
}

}//namespace XAXA