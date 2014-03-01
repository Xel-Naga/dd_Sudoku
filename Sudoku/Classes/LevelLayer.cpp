
#include "LevelLayer.h"
#include "HelloWorldScene.h"
#include "ScoreMgr.h"

#include <sstream>
#include <fstream>
#include "json/json.h"

const int LEVEL_PER_LINE = 4;

USING_NS_CC_EXT;
namespace XAXA {

void add_level(int level[SUDOKU_GRID][SUDOKU_GRID],int level_win[SUDOKU_GRID][SUDOKU_GRID]) {
    SudokuLevelMap* new_level = new SudokuLevelMap(level);
    new_level->set_level_win(level_win);
    LevelMgr::instance()->add_level(new_level);
}

void loadLevel() {
    int level[SUDOKU_GRID][SUDOKU_GRID];
    int level_win[SUDOKU_GRID][SUDOKU_GRID];
    std::ifstream ifs;
    ifs.open("level.json");
    assert(ifs.is_open());
 
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    {
        return ;
    }
 
    int size = root.size();
    for (int i=0; i<size; ++i)
    {
        int level_sn = root[i]["Level"].asInt();
        std::string map = root[i]["map"].asString();
        //CCLOG("map:%s",map.c_str());
        std::stringstream mapstream(map);
        char seperator;

        for(int j = 0; j < SUDOKU_GRID*SUDOKU_GRID; ++j) {
            int height = j/SUDOKU_GRID;
            int width = j%SUDOKU_GRID;
            mapstream>>level[height][width];
            //CCLOG("%d",level[height][width]);
            mapstream>>seperator;
        }

        std::string map_win = root[i]["map_win"].asString();
        //CCLOG("map_win:%s",map_win.c_str());
        mapstream.str(map_win);
        for(int j = 0; j < SUDOKU_GRID*SUDOKU_GRID; ++j) {
            int height = j/SUDOKU_GRID;
            int width = j%SUDOKU_GRID;
            mapstream>>level_win[height][width];
            //CCLOG("%d",level_win[height][width]);
            mapstream>>seperator;
        }

        add_level(level,level_win);
    }
}

class LevelBtn: public CCNode {
public:
    LevelBtn(int i, const char* bg, const char* bg_hi,ccColor3B color, float fontSize) {
        
        char buff[8];
        sprintf(buff,"%d",i);
        _btnLevel = createButtonWithTitle(buff,bg,bg_hi,color,fontSize);
        addChild(_btnLevel,Z_GAME_LAYER,i);
        _btnLevel->setPreferredSize(CCSize(128,128));
        _btnLevel->setScale(0.7f);
        _btnLevel->addTargetWithActionForControlEvents(this, cccontrol_selector(LevelBtn::onPressBtn), CCControlEventTouchUpInside);  

        //setContentSize(CCSize(128,128));

        _iconLock = CCScale9Sprite::create(s_lock_0);
        _iconLock->setScale(0.4f);
        addChild(_iconLock,Z_GAME_LAYER+1);
        //iconLock->setPosition(128,128);
        _isLocked = true;
    }

    void setLocked(bool needLock) {
        _isLocked = needLock;

        if(needLock) {
            _iconLock->setVisible(true); 
            _btnLevel->setTouchEnabled(false);
        }
        else {
            _iconLock->setVisible(false);
            _btnLevel->setTouchEnabled(true);
        }
    }

protected:
    void onPressBtn(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
        if(!_isLocked) {
            CCControlButton* btn = (CCControlButton*)pSender;
            goToLevel(btn->getTag());
        }
    }

    void goToLevel(LEVEL_SN_TYPE sn) {
    XAXA::LevelMgr::instance()->set_curr_level(sn);
    CCScene* se=HelloWorld::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.2f, se));
}

    CCControlButton * _btnLevel;
    CCScale9Sprite * _iconLock;
    bool _isLocked;
};

//非常重要，CCB中的自定义类不会再调用其init函数，需要在onNodeLoaded中手工调用一下
bool LevelLayer::init() {
    LevelMgr::instance()->clear();
    loadLevel();
    LevelMgr::instance()->loadLevelInfo();

    char buff[8];
    for(int i = 0; i < LevelMgr::instance()->get_level_count(); i++) {
        LevelBtn * level = new LevelBtn(i,s_BtnLevelNormal,s_BtnLevelNormal,ccc3(0,0,255),20);
        addChild(level,Z_GAME_LAYER,i);
        float width = 128*0.7f;
        float x = 20+width*0.5f+(width*0.7f)*(i%LEVEL_PER_LINE);
        float y = (360 - (width*0.7f)*(i/LEVEL_PER_LINE));
        level->setPosition(x,y);
        CCLOG("level position:%0.1f,%0.1f",x,y);
        LevelInfo* lvlInfo = LevelMgr::instance()->get_level_info(i);
        if(i < 3) {
            level->setLocked(false); 
        }
        else {
            level->setLocked(lvlInfo->is_lock);
        }
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
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, se));
    //se->autorelease();
}

void LevelLayer::onPressLevelBtn(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    CCControlButton* btn = (CCControlButton*)pSender;
    goToLevel(btn->getTag());
}

}//end namespace XAXA
