#ifndef _LEVELLAYER_H_
#define _LEVELLAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "LevelMgr.h"
#include "Resource.h"

USING_NS_CC;
//暂时给一个游戏引擎取个名字XAXA，意思就是HAHA(哈哈)
namespace XAXA {

class SudokuLevelMap:public LevelMap {
public:
    SudokuLevelMap(int level[SUDOKU_GRID][SUDOKU_GRID]) {
        memcpy(&_level,level,sizeof(int)*SUDOKU_GRID*SUDOKU_GRID);
    }

    int* get_level() { return (int*)_level; }

    int _level[SUDOKU_GRID][SUDOKU_GRID];
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


    void onPressLevelBtn1(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onPressLevelBtn2(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onPressLevelBtn3(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onPressLevelBtn4(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);

    void goToLevel(LEVEL_SN_TYPE sn);
private:
    cocos2d::extension::CCControlButton * mBtnLvl1;
    cocos2d::extension::CCControlButton * mBtnLvl2;
    cocos2d::extension::CCControlButton * mBtnLvl3;
    cocos2d::extension::CCControlButton * mBtnLvl4;
};


}//end namespace XAXA

#endif
