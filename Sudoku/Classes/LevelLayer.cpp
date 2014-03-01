
#include "LevelLayer.h"
#include "HelloWorldScene.h"
#include "ScoreMgr.h"

USING_NS_CC_EXT;
namespace XAXA {

void add_level(int level[SUDOKU_GRID][SUDOKU_GRID],int level_win[SUDOKU_GRID][SUDOKU_GRID]) {
    SudokuLevelMap* new_level = new SudokuLevelMap(level);
    new_level->set_level_win(level_win);
    LevelMgr::instance()->add_level(new_level);
}

void add_level1() {
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

    int level_win[SUDOKU_GRID][SUDOKU_GRID] = { 7,9,8,5,4,1,3,2,6,
                                            4,5,3,6,2,9,8,1,7,
                                            6,2,1,3,8,7,9,4,5,
                                            5,4,9,2,1,8,6,7,3,
                                            1,7,6,4,5,3,2,9,8,
                                            8,3,2,9,7,6,4,5,1,
                                            3,1,4,8,9,5,7,6,2,
                                            2,6,7,1,3,4,5,8,9,
                                            9,8,5,7,6,2,1,3,4
    };

    add_level(level,level_win);
}

void add_level2() {
    int level[SUDOKU_GRID][SUDOKU_GRID] = { 3,6,255,255,7,255,255,255,5,
                                            255,255,255,6,2,255,255,255,255,
                                            255,255,1,255,255,255,255,7,255,
                                            4,9,255,255,255,255,255,255,255,
                                            255,255,255,255,255,8,255,4,255,
                                            255,7,255,3,5,255,9,255,255,
                                            7,255,255,1,255,255,5,255,4,
                                            255,255,6,255,4,255,255,255,1,
                                            255,2,255,255,255,9,3,255,255,
    };

    int level_win[SUDOKU_GRID][SUDOKU_GRID] = { 3,6,8,9,7,1,4,2,5,
                                                9,4,7,6,2,5,1,3,8,
                                                2,5,1,4,8,3,6,7,9,
                                                4,9,3,7,1,6,8,5,2,
                                                6,1,5,2,9,8,7,4,3,
                                                8,7,2,3,5,4,9,1,6,
                                                7,8,9,1,3,2,5,6,4,
                                                5,3,6,8,4,7,2,9,1,
                                                1,2,4,5,6,9,3,8,7,
    };

    add_level(level,level_win);
}

void add_level3() {
    int level[SUDOKU_GRID][SUDOKU_GRID] = { 255,255,7,255,255,255,255,255,255,
                                            3,255,255,255,4,1,255,2,255,
                                            8,1,255,2,255,255,255,9,255,
                                            255,255,255,255,255,255,7,255,255,
                                            6,255,255,255,255,9,255,1,255,
                                            255,255,255,3,8,7,255,255,255,
                                            255,255,9,255,3,6,255,7,255,
                                            255,255,255,255,255,255,8,255,3,
                                            255,255,6,255,255,4,255,255,255,
    };

    int level_win[SUDOKU_GRID][SUDOKU_GRID] = { 2,6,7,5,9,8,4,3,1,
                                                3,9,5,7,4,1,6,2,8,
                                                8,1,4,2,6,3,5,9,7,
                                                9,4,3,6,1,2,7,8,5,
                                                6,7,8,4,5,9,3,1,2,
                                                1,5,2,3,8,7,9,4,6,
                                                5,8,9,1,3,6,2,7,4,
                                                4,2,1,9,7,5,8,6,3,
                                                7,3,6,8,2,4,1,5,9,
    };

    add_level(level,level_win);
}

void add_level4() {
    int level[SUDOKU_GRID][SUDOKU_GRID] = { 255,255,255,255,255,255,4,3,255,
                                            255,255,255,255,1,255,6,2,255,
                                            255,8,1,255,255,255,255,255,255,
                                            7,255,255,9,255,255,255,255,255,
                                            255,3,255,7,5,255,9,6,255,
                                            255,1,255,255,255,255,255,255,3,
                                            255,7,5,255,255,9,2,255,255,
                                            255,255,255,255,255,255,255,255,255,
                                            6,255,255,8,3,5,255,7,255,
    };

    int level_win[SUDOKU_GRID][SUDOKU_GRID] = { 2,6,7,5,9,8,4,3,1,
                                                9,5,3,4,1,7,6,2,8,
                                                4,8,1,3,2,6,5,9,7,
                                                7,2,6,9,4,3,8,1,5,
                                                8,3,4,7,5,1,9,6,2,
                                                5,1,9,6,8,2,7,4,3,
                                                3,7,5,1,6,9,2,8,4,
                                                1,9,8,2,7,4,3,5,6,
                                                6,4,2,8,3,5,1,7,9,
    };

    add_level(level,level_win);
}

class LevelBtn: public CCNode {
    LevelBtn() {
        btnLevel = createButtonWithTitle("1",s_BtnLevelNormal,s_BtnLevelNormal,ccc3(0,0,255),20);
        addChild(btnLevel,Z_GAME_LAYER);
        btnLevel->setPosition(160,160);
        btnLevel->setPreferredSize(CCSize(128,128));
        btnLevel->addTargetWithActionForControlEvents(this, cccontrol_selector(LevelLayer::onPressLevelBtn), CCControlEventTouchUpInside);  
    }
    CCControlButton * btnLevel;
};

//非常重要，CCB中的自定义类不会再调用其init函数，需要在onNodeLoaded中手工调用一下
bool LevelLayer::init() {
    add_level1();
    LevelMgr::instance()->get_level_info(0)->is_lock = false;
    add_level2();
    LevelMgr::instance()->get_level_info(1)->is_lock = false;
    add_level3();
    add_level4();

    char buff[8];
    for(int i = 0; i < LevelMgr::instance()->get_level_count(); i++) {
        sprintf(buff,"%d",i);
        CCControlButton * lv1 = createButtonWithTitle(buff,s_BtnLevelNormal,s_BtnLevelNormal,ccc3(0,0,255),20);
        addChild(lv1,Z_GAME_LAYER,i);
        lv1->setPreferredSize(CCSize(128,128));
        lv1->setScale(0.7f);
        float width = 128*0.7f;
        float x = 10+width*0.5f+(width*0.7f)*(i%3);
        float y = (360 - (width*0.7f)*(i/3));
        lv1->setPosition(x,y);
        CCLOG("level position:%0.1f,%0.1f",x,y);
        lv1->addTargetWithActionForControlEvents(this, cccontrol_selector(LevelLayer::onPressLevelBtn), CCControlEventTouchUpInside);  
    }
    return true;
}

void LevelLayer::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader) {
    //init();
}


SEL_MenuHandler LevelLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
    return NULL;    
}

SEL_CCControlHandler LevelLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {

    return NULL;
}

bool LevelLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) {

    return false;
}

void LevelLayer::goToLevel(LEVEL_SN_TYPE sn) {
    XAXA::LevelMgr::instance()->set_curr_level(sn);
    CCScene* se=HelloWorld::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.2f, se));
    //se->autorelease();
}

void LevelLayer::onPressLevelBtn(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    CCControlButton* btn = (CCControlButton*)pSender;
    goToLevel(btn->getTag());
}

}//end namespace XAXA
