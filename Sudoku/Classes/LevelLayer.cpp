
#include "LevelLayer.h"
#include "HelloWorldScene.h"
USING_NS_CC_EXT;
namespace XAXA {

void add_level1() {
    int level[SUDOKU_GRID][SUDOKU_GRID] = { 255,255,8,255,4,255,3,255,255,
                                        4,255,255,6,255,9,255,255,7,
                                        6,255,255,255,255,255,255,255,5,
                                        5,255,9,255,1,255,6,255,3,
                                        255,255,6,255,255,255,2,255,255,
                                        8,255,2,255,7,255,4,255,1,
                                        3,255,255,255,255,255,255,255,2,
                                        2,255,255,1,255,4,255,255,9,
                                        255,255,5,255,6,255,1,255,255
    };

    LevelMgr::instance()->add_level(new SudokuLevelMap(level));
}

void add_level2() {
    int level[SUDOKU_GRID][SUDOKU_GRID] = { 7,9,8,5,4,1,3,2,6,
                                            4,5,3,6,2,9,8,1,7,
                                            6,2,1,3,8,7,9,4,5,
                                            5,4,9,2,1,8,6,7,3,
                                            1,7,6,4,5,3,2,9,8,
                                            8,3,2,9,7,6,4,5,1,
                                            3,1,4,8,9,5,7,6,2,
                                            2,6,7,1,3,4,5,8,9,
                                            9,8,5,7,6,2,1,3,255
    };

    LevelMgr::instance()->add_level(new SudokuLevelMap(level));
}

//非常重要，CCB中的自定义类不会再调用其init函数，需要在onNodeLoaded中手工调用一下
bool LevelLayer::init() {
    add_level1();
    LevelMgr::instance()->get_level_info(0)->is_lock = false;
    add_level2();
    LevelMgr::instance()->get_level_info(1)->is_lock = false;
    add_level2();
    add_level2();
    add_level2();

    return true;
}

void LevelLayer::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader) {
    init();
}


SEL_MenuHandler LevelLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
    return NULL;    
}

SEL_CCControlHandler LevelLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPressLevelBtn1", LevelLayer::onPressLevelBtn1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPressLevelBtn2", LevelLayer::onPressLevelBtn2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPressLevelBtn3", LevelLayer::onPressLevelBtn3);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPressLevelBtn4", LevelLayer::onPressLevelBtn4);

    return NULL;
}

bool LevelLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) {
    //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnLvl1", CCControlButton *, this->mBtnLvl1);
   // CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnLvl2", CCControlButton *, this->mBtnLvl2);
  //  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnLvl3", CCControlButton *, this->mBtnLvl3);
   // CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnLvl4", CCControlButton *, this->mBtnLvl4);

    return false;
}

void LevelLayer::goToLevel(LEVEL_SN_TYPE sn) {
    XAXA::LevelMgr::instance()->set_curr_level(sn);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.2f, HelloWorld::scene()));
}

void LevelLayer::onPressLevelBtn1(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    switch(pCCControlEvent) {
        case CCControlEventTouchDown:
            break;
        case CCControlEventTouchDragInside:
            
            break;
        case CCControlEventTouchDragOutside:
            
            break;
        case CCControlEventTouchDragEnter:
           
            break;
        case CCControlEventTouchDragExit:
            
            break;
        case CCControlEventTouchUpInside:
            goToLevel(0);
            break;
        case CCControlEventTouchUpOutside:
            
            break;
        case CCControlEventTouchCancel:
            
            break;
        case CCControlEventValueChanged:
            
            break;
        default:
            assert(false); // OH SHIT!
    }
}

void LevelLayer::onPressLevelBtn2(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    switch(pCCControlEvent) {
        case CCControlEventTouchUpInside:
            goToLevel(1);            
            break;
        default:
            assert(false); // OH SHIT!
    }
}

void LevelLayer::onPressLevelBtn3(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    switch(pCCControlEvent) {
        case CCControlEventTouchUpInside:
            goToLevel(1); 
            break;
        default:
            assert(false); // OH SHIT!
    }
}

void LevelLayer::onPressLevelBtn4(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    switch(pCCControlEvent) {
        case CCControlEventTouchUpInside:
            goToLevel(1); 
            break;
        default:
            assert(false); // OH SHIT!
    }
}


}//end namespace XAXA
