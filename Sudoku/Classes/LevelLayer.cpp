
#include "LevelLayer.h"
#include "HelloWorldScene.h"
#include "ScoreMgr.h"
#include "MainLayer.h"
#include "Utils.h"
#include "platform/CCFileUtils.h"



#include <sstream>
#include <fstream>
#include "json/json.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

const int LEVEL_PER_ROW = 4;
const int LEVEL_PER_COL = 5;

USING_NS_CC_EXT;
namespace XAXA {

void add_level(int level[SUDOKU_GRID][SUDOKU_GRID],int level_win[SUDOKU_GRID][SUDOKU_GRID],int lock) {
    SudokuLevelMap* new_level = new SudokuLevelMap(level);
    new_level->set_level_win(level_win);
    LevelMgr::instance()->add_level(new_level,lock);
}

void LevelLayer::loadLevel() {
    int level[SUDOKU_GRID][SUDOKU_GRID];
    int level_win[SUDOKU_GRID][SUDOKU_GRID];
    std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename( "level.json" );
    unsigned long fileSize = 0;
    unsigned char * fileContents = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(),"r", &fileSize );

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse((char*)fileContents, root, false))
    {
        CCLOG("loadLevel() parse json failed");
        return ;
    }
 
    int size = root.size();
    CCLOG("loadLevel() size:%d",size);
    //size = 20;
    for (int i=0; i<size; ++i)
    {
        int level_sn = root[i]["level"].asInt();
        int lock = root[i]["lock"].asInt();
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

        add_level(level,level_win,lock);
    }
}

class LevelBtn: public CCNode {
public:
    LevelBtn(int i, const char* bg, const char* bg_hi,ccColor3B color, float fontSize) {
        
        char buff[8];
        sprintf(buff,"%d",i+1);
        _btnLevel = createButtonWithTitle(buff,bg,bg_hi,color,fontSize);
        addChild(_btnLevel,Z_GAME_LAYER,i);
        _btnLevel->setPreferredSize(CCSize(64,64));
        //_btnLevel->setScale(0.7f);
        _btnLevel->addTargetWithActionForControlEvents(this, cccontrol_selector(LevelBtn::onPressBtn), CCControlEventTouchUpInside);  

        //setContentSize(CCSize(128,128));

        _iconLock = CCScale9Sprite::create(s_lock_0);
        _iconLock->setScale(1.2f);
        addChild(_iconLock,Z_GAME_LAYER+1);
        //iconLock->setPosition(128,128);

        addImageToSpriteCache(s_check);
        _levelPassed = CCScale9Sprite::create(s_check);
        _levelPassed->setScale(1.2f);
        _levelPassed->setVisible(false);
        addChild(_levelPassed,Z_GAME_LAYER+2);
        //小勾放在右上角
        //_levelPassed->setAnchorPoint(ccp(-0.2,0.8));
        //_levelPassed->setPosition(0,0);
        
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

    void setPassed(bool isPassed) {
        _levelPassed->setVisible(isPassed); 
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
    CCScale9Sprite * _levelPassed;
    bool _isLocked;
};

//非常重要，CCB中的自定义类不会再调用其init函数，需要在onNodeLoaded中手工调用一下
bool LevelLayer::init() {
    LevelMgr::instance()->clear();
    loadLevel();
    LevelMgr::instance()->loadLevelInfo();

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();  
    float scaleXValue = 1.0f;
    float scaleYValue = 0.85f;
    int count = LevelMgr::instance()->get_level_count();
    //这里只是一个巧合，图片比较大，中间的球比较小，可能只占图片的70%左右，所以布局的时候按球的大小来计算
    float width = 64;
    float scrollViewWidth = (width*scaleXValue)*(LEVEL_PER_ROW);
    float scrollViewHeight = (width*scaleYValue)*(LEVEL_PER_COL);
    //当前关卡最多有多少行，根据每行的关卡数来定
    _colTotal = count/LEVEL_PER_ROW+1;
    _colCurrent = loadCurrentCol();
    if(_colCurrent == 255) {
        _colCurrent = _colTotal - LEVEL_PER_COL;
    }
    CCLOG("scrollViewWidth:%0.1f, scrollViewHeight:%0.1f,col:%d",scrollViewWidth,scrollViewHeight,_colTotal);

    //垂直滚动，所以可视区和实际数据区的宽度是一样的
    float scrollWidth = scrollViewWidth;
    float scrollHeight = (width*scaleYValue)*(_colTotal);
    scrollHeight = scrollHeight > scrollViewHeight? scrollHeight:scrollViewHeight;
    //_ScrollView->setContentSize(CCSizeMake(scrollWidth,scrollHeight));
    CCLOG("scrollWidth:%0.1f, scrollHeight:%0.1f",scrollWidth,scrollHeight);

    //可视区域的一定要<=容器大小
    //这里是在容器层testScrollView里创建scrollView的实例 
    //这里的size是可视区域的大小，也就是超过这个范围的内容会被隐去
    CCLayerColor* container = CCLayerColor::create(ccc4(0,0,255,1));
    container->setContentSize(CCSizeMake(scrollWidth,scrollHeight));

    //滚动条
    addImageToSpriteCache(s_scroll_bar);
    addImageToSpriteCache(s_scroll_slot);
    //_ScrollView=NCSScrollViewWithBar::create(container,CCSizeMake(scrollViewWidth,scrollViewHeight),
    //    s_scroll_slot,s_scroll_bar);
    _ScrollView=CCScrollView::create(CCSizeMake(scrollViewWidth,scrollViewHeight),container);

    _ScrollView->setDirection(kCCScrollViewDirectionVertical);
    addChild(_ScrollView,Z_HUD_LAYER);  
    //_ScrollView->setAnchorPoint(ccp(0.0,0.0));
    CCPoint scrollPos = CCPoint((winSize.width-scrollViewWidth)/2,(winSize.height-scrollViewHeight)/2-20);
    //_ScrollView->setAnchorPoint(ccp(0.5,0.5));
    //CCPoint scrollPos = CCPoint(winSize.width/2,winSize.height/2);
    _ScrollView->setPosition(scrollPos);
    CCPoint minPos = _ScrollView->minContainerOffset();
    CCPoint maxPos = _ScrollView->maxContainerOffset();
    CCLog("minPos: %0.1f,%0.1f maxPos: %0.1f, %0.1f",minPos.x,minPos.y,maxPos.x,maxPos.y);
    //_ScrollView->setContentOffset(minPos);
    _ScrollView->setContentOffset(ccp(minPos.x,(-1)*_colCurrent*width*scaleYValue));
    _ScrollView->setBounceable(false);
    _ScrollView->setTouchEnabled(false);
    CCLOG("ScrollView pos:%0.1f, %0.1f",_ScrollView->getPosition().x,_ScrollView->getPosition().y);
    
    for(int i = count; i > 0; i--) {
        LevelBtn * level = new LevelBtn(i-1,s_BtnLevelNormal,s_BtnLevelNormal,ccc3(0,0,255),20);
        _ScrollView->addChild(level,Z_GAME_LAYER,i-1);
        
        float x = width*scaleXValue/2 + (width*scaleXValue)*((i-1)%LEVEL_PER_ROW);
        float y = width*scaleYValue/2 + (width*scaleYValue)*((count)/LEVEL_PER_ROW -  (i-1)/LEVEL_PER_ROW);
        level->ignoreAnchorPointForPosition(false);
        level->setAnchorPoint(ccp(0.0,0.0));
        level->setPosition(x,y);
        level->setScale(scaleXValue);
        CCLOG("level %d position:%0.1f,%0.1f",i,x,y);
        LevelInfo* lvlInfo = LevelMgr::instance()->get_level_info(i-1);
        level->setLocked(lvlInfo->is_lock);
        level->setPassed(lvlInfo->is_win);
    }
    
    CCLOG("LevelLayer: win size:%0.1f,%0.1f content size:%0.1f,%0.1f",winSize.width,winSize.height,
        _ScrollView->getContentSize().width,_ScrollView->getContentSize().height);

    //增加上下选择按钮
    addImageToSpriteCache(s_up_arrow);
    CCControlButton* upBtn = createButtonWithTitle("",s_up_arrow,s_up_arrow);
    addChild(upBtn,Z_GAME_LAYER);
    upBtn->setPreferredSize(CCSize(64,24));
    upBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LevelLayer::onPressUpBtn), CCControlEventTouchUpInside);  
    upBtn->setAnchorPoint(ccp(0.5,0));
    upBtn->setPosition(winSize.width/2,scrollPos.y + scrollViewHeight -5);

    addImageToSpriteCache(s_down_arrow);
    CCControlButton* downBtn = createButtonWithTitle("",s_down_arrow,s_down_arrow);
    addChild(downBtn,Z_GAME_LAYER);
    downBtn->setPreferredSize(CCSize(64,24));
    downBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(LevelLayer::onPressDownBtn), CCControlEventTouchUpInside);  
    downBtn->setAnchorPoint(ccp(0.5,0));
    downBtn->setPosition(winSize.width/2,scrollPos.y - downBtn->getContentSize().height/2 - 5);
    
    return true;
}

void LevelLayer::onPressBack(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
    REPLACE_SCENE_FROM_CCBI(MainLayer);
}

void LevelLayer::saveCurrentCol() {
    ScoreMgr::instance()->saveInt("level_layer_current_col", _colCurrent);
}

int LevelLayer::loadCurrentCol() {
    return ScoreMgr::instance()->loadInt("level_layer_current_col",255);
}

void LevelLayer::onPressUpBtn(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    if(_colCurrent +1 > _colTotal - LEVEL_PER_COL) {
        SimpleAudioEngine::sharedEngine()->playEffect(FAIL_FILE);
        return;
    }
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
    _colCurrent++;
    
    float width = -64;
    float scaleY = 0.85f;
    CCPoint pos = _ScrollView->getContentOffset();
    _ScrollView->setContentOffset(ccp(pos.x,_colCurrent*width*scaleY));
    saveCurrentCol();
}

void LevelLayer::onPressDownBtn(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    if(_colCurrent -1 < 0) {
        SimpleAudioEngine::sharedEngine()->playEffect(FAIL_FILE);
        return;
    }
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
    _colCurrent--;
    float width = -64;
    float scaleY = 0.85f;
    CCPoint pos = _ScrollView->getContentOffset();
    _ScrollView->setContentOffset(ccp(pos.x,_colCurrent*width*scaleY));
    saveCurrentCol();
}

void LevelLayer::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader) {
    //init();
}


SEL_MenuHandler LevelLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
    return NULL;    
}

SEL_CCControlHandler LevelLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPressBack",LevelLayer::onPressBack);
    return NULL;
}

bool LevelLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) {

    return false;
}

void LevelLayer::goToLevel(LEVEL_SN_TYPE sn) 
{
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
    XAXA::LevelMgr::instance()->set_curr_level(sn);
    CCScene* se=HelloWorld::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, se));
    //se->autorelease();
}

void LevelLayer::onPressLevelBtn(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
    CCControlButton* btn = (CCControlButton*)pSender;
    goToLevel(btn->getTag());
}

void LevelLayer::scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {
/*
    CCPoint pos = view->getContentOffset();
    float scaleY = 0.85f;
    float height = 64;
    if(pos.y / (height*scaleY)) {

    }
    */
}

}//end namespace XAXA
