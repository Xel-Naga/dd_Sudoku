#ifndef _LEVELLAYER_H_
#define _LEVELLAYER_H_

#include "cocos2d.h"
#include "CCCamera.h"
#include "cocos-ext.h"
#include "LevelMgr.h"
#include "Resource.h"
#include "Utils.h"

USING_NS_CC;
//暂时给一个游戏引擎取个名字XAXA，意思就是HAHA(哈哈)
namespace XAXA {

class SudokuLevelMap:public LevelMap {
public:
    SudokuLevelMap(int level[SUDOKU_GRID][SUDOKU_GRID]) {
        memcpy(&_level,level,sizeof(int)*SUDOKU_GRID*SUDOKU_GRID);
    }

    void set_level_win(int level[SUDOKU_GRID][SUDOKU_GRID]) {
        memcpy(&_level_win,level,sizeof(int)*SUDOKU_GRID*SUDOKU_GRID);
    }

    int* get_level_win() { return (int*)_level_win;}

    int* get_level() { return (int*)_level; }

    int _level[SUDOKU_GRID][SUDOKU_GRID];
    int _level_win[SUDOKU_GRID][SUDOKU_GRID];
};


class LevelLayer
:public CCLayer 
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener {
public:
    LevelLayer() { CCLog("LevelLayer::Ctor()");}
    virtual ~LevelLayer() { CCLog("LevelLayer::Dtor()");}

    bool init();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(LevelLayer, create);

    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual void onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);

    void onPressLevelBtn(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);

    void goToLevel(LEVEL_SN_TYPE sn);
    void menuGotoMainMenu(CCObject* pSender);

    void loadLevel();
};

CREATE_AUTORELEASE_LOADER(LevelLayer);

}//end namespace XAXA

#endif
