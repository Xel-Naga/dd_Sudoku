#include "LevelMgr.h"
#include "MainLayer.h"
#include "Resource.h"
#include "ScoreMgr.h"

#include "WapsAd.h"

#include "LevelLayer.h"
USING_NS_CC_EXT;

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

namespace XAXA {

void MainLayer::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader) {
    init();
}

SEL_CCControlHandler MainLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPressPlay",MainLayer::onPressPlay);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPressOption", MainLayer::onPressOption);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPressExit", MainLayer::onPressExit);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPressMore", MainLayer::onPressMore);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPressSpeaker", MainLayer::onPressSpeaker);

    return NULL;
}

bool MainLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) {
    return false;
}

SEL_MenuHandler MainLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
    return NULL;
}

void MainLayer::onPressPlay(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
    REPLACE_SCENE_FROM_CCBI(LevelLayer);
}

void MainLayer::onPressMore(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
    WapsAd::showAd(0);
}

void MainLayer::onPressOption(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
    WapsAd::showAd(14);
}

void MainLayer::onPressExit(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
    WapsAd::showAd(12);
	CCDirector::sharedDirector()->end();
}

void MainLayer::saveSpeakerState(bool isOn) {
    if(isOn) {
        ScoreMgr::instance()->saveInt("speaker_state",1);
    }
    else {
        ScoreMgr::instance()->saveInt("speaker_state",0);
    }
}

bool MainLayer::getSpeakerState() {
    return ScoreMgr::instance()->loadInt("speaker_state")> 0;
}

void MainLayer::onPressSpeaker(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    static bool is_music_pauseing = false;
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
    if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.3f);
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_FILE, true);
        saveSpeakerState(true);
        is_music_pauseing = false;
    }
    else if(!is_music_pauseing)
    {
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
        saveSpeakerState(false);
        is_music_pauseing = true;
    }
    else {
        SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
        saveSpeakerState(true);
        is_music_pauseing = false;
    }

    WapsAd::showAd(11);
}

void MainLayer::initSound() {

    // preload background music and effect
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic( MUSIC_FILE );
    SimpleAudioEngine::sharedEngine()->preloadEffect(CLICK_FILE);
    
    // set default volume
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5f);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.3f);

    if(getSpeakerState() && !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_FILE, true);
    }
}

void MainLayer::initLevel() {
    //¹Ø¿¨³õÊ¼»¯
    //XAXA::LevelLayer* curr_level = XAXA::LevelLayer::create(); 
}

bool MainLayer::init() {
    if(!CCLayer::init()) {
        return false;
    }

    initSound();

    initLevel();

    return true;
}

}//namespace XAXA