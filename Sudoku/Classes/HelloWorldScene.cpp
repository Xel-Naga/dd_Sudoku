
#include "actions\CCActionInterval.h"
#include "chipmunk.h"

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include <stdio.h>
#include <string.h>
#include <list>
#include <map>

//调用广告的接口
#include "WapsAd.h"

USING_NS_CC;

#include "LevelLayer.h"
#include "LevelMgr.h"
#include "MainLayer.h"
#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "Resource.h"
#include "Utils.h"
#include "ScoreMgr.h"

namespace XAXA {

enum {
	kTagParentNode = 1,
    kTagProgresBar,
    ktagHilite,
    kTagGameSceneMenu,
};

enum {
	Z_PHYSICS_DEBUG = 100,
};


CCSize g_gridSize;

#pragma pack(1)

typedef struct {
	int x;
	int y;
	int value;
}GridInfo;
#pragma pack()

class Grid
{
public:
	Grid(int value=255, bool isStatic=false,CCTexture2D* pTexture=NULL) :_value(value),_isStatic(isStatic) 
	{
		_pValueLabel = CCLabelAtlas::create("",s_inkNumber,20,32,'0');
		setValue(value);
		if(_isStatic) {
			_pValueLabel->setColor(ccc3(125,125,125));
		}
        else {
            _pValueLabel->setColor(ccc3(25,25,25));
        }

        _isRevealed = false;

		if(pTexture) {

            _pHighlight = CCScale9Sprite::createWithSpriteFrameName(s_circle);
            _pHighlight->setAnchorPoint(ccp(0.5f,0.5f));
			_pHighlight->setPosition(g_gridSize.width/2-3,g_gridSize.height/2);
            _pHighlight->setVisible(false);

			_pValueLabel->setAnchorPoint(ccp(0.5f,0.5f));
			_pValueLabel->setPosition(g_gridSize.width/2-3,g_gridSize.height/2);

            _pGrid = CCScale9Sprite::createWithSpriteFrameName(s_grid);
			_pGrid->setContentSize(g_gridSize);
            
            _pGrid->addChild(_pValueLabel,Z_HUD_LAYER);
            _pGrid->addChild(_pHighlight,Z_BACKGROUND,ktagHilite);
		}
	}

    void hilite(bool on) {
        if(on) {
            _pHighlight->setVisible(true);
        }
        else {
            _pHighlight->setVisible(false);
        }
    }

	void setPosition(const CCPoint& pos) {
		if(_pGrid) {
			_pGrid->setPosition(pos); 
		}
		else {
			_pValueLabel->setPosition(pos);
		}
	}

    void setLayout(int row, int col) {
        _row = row;
        _col = col;
    }

    CCPoint getLayout() {
        return CCPoint(_col,_row);//列是宽，行是高
    }

	CCNode* GetNode() {
		if(_pGrid) {
			return _pGrid;
		}
		else {
			return _pValueLabel;
		}
	}

	void setValue(int value) {
		_value = value;
		char strValue[16]={0};
		if(value != 255) {
			sprintf(strValue, "%d", value);
		}

		_pValueLabel->setString(strValue);
	}

	int getValue() {
		return _value;
	}

	bool IsStatic() {
		return _isStatic;
	}

    bool isRevealed() {
        return _isRevealed;
    }

    void setRevealed(bool isRevealed) {
        if(isRevealed) {
            _isRevealed = true;
            _pValueLabel->setColor(ccc3(150,0,0));
        }
    }

	static Grid* GridWithTexture(CCTexture2D* aTexture,int value=255, bool isStatic=false);

private:
    int _row;//行,从0开始
    int _col;//列，从0开始
    bool _isRevealed;//被照过了，不能再改也不能再照了
	int _value;
	bool _isStatic;//如果是静态初始数据，则不允许修改
	CCLabelAtlas* _pValueLabel;
	CCScale9Sprite* _pGrid;
    CCScale9Sprite* _pHighlight;
};


Grid* Grid::GridWithTexture(CCTexture2D* aTexture,int value, bool isStatic)
{
	Grid* pGrid = new Grid(value,isStatic,aTexture);
	return pGrid;
}

Grid* g_GridList[SUDOKU_GRID*SUDOKU_GRID];

//锚点为(0,0)
class InputPad: public CCLayerColor{
public:
	CREATE_FUNC(InputPad);

	InputPad()
    {
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        _padding = 2;
        _pLastGrid = NULL;
        //这里如果使用CCScale9Sprite，后面遍历子对象时会有问题，因为它内部还包含了一个自己的对象，需要过滤掉
		_pad = CCSprite::createWithSpriteFrameName(s_InputPad);
        _pad->setAnchorPoint(ccp(0.5,0.5));
        _pad->setPosition(ccp(visibleSize.width/2,_pad->getContentSize().height/2));
        //_pad->setPosition(-64,_pad->getContentSize().height/2);
        this->addChild(_pad,Z_INPUTPAD);
		CCLOG("visibleSize size:%0.1f,%0.1f",visibleSize.width,visibleSize.height);
	    CCLOG("_pad pos:%0.1f,%0.1f",_pad->getPosition().x,_pad->getPosition().y);
        int rowNum = 1;

        int colNum = 9;
        char buf[8];
        int number;
        float pad_width = 0;
        CCSize backgroundButtonSize;
		for(int i=0;i<rowNum;i++) {
			for(int j=0;j<colNum;j++) {

                number = i*colNum + j+1;
                sprintf(buf,"%d",number);
                CCScale9Sprite *backgroundButton = CCScale9Sprite::create(s_grid);
                CCScale9Sprite *backgroundHighlightedButton = CCScale9Sprite::create(s_grid);
                backgroundButtonSize = backgroundButton->getContentSize();
                //CCLOG("backgroud width:%0.1f",backgroundButton->getContentSize().width);
                //CCLOG("backgroudhi width:%0.1f",backgroundHighlightedButton->getContentSize().width);
                CCLabelAtlas *titleButton = CCLabelAtlas::create(buf,s_inkNumber,20,32,'0');
                //CCLOG("title width:%0.1f",titleButton->getContentSize().width);
                titleButton->setColor(ccc3(0, 0, 0));
                CCControlButton *controlButton = CCControlButton::create(titleButton, backgroundButton);
                controlButton->setBackgroundSpriteForState(backgroundHighlightedButton, CCControlStateHighlighted);
                controlButton->setTitleColorForState(ccWHITE, CCControlStateHighlighted);
                controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(InputPad::TouchUpInside), CCControlEventTouchUpInside);
                //controlButton->setContentSize(backgroundButtonSize);
                //CCLOG("btn width:%0.1f",backgroundButtonSize.width);
                pad_width = (backgroundButtonSize.width*controlButton->getScaleX()+_padding)*colNum;
                //CCLOG("pad width:%0.1f",pad_width);
                _pad->addChild(controlButton, Z_TOP_LAYER,number);
			}
		}

        float x = visibleSize.width/2 - pad_width/2+32;
        
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(_pad->getChildren(), pObject)
        {
            CCNode* pChild = dynamic_cast<CCNode*>(pObject);
            if (pChild)
            {
                float pos = x + backgroundButtonSize.width * pChild->getScaleX() / 2.0f;
                CCLOG("child pos:%0.1f,%0.1f",pos,_pad->getContentSize().height/2);
                pChild->setAnchorPoint(ccp(0.5,0.5));
                pChild->setPosition(ccp(pos, _pad->getContentSize().height/2));
                x += backgroundButtonSize.width * pChild->getScaleX() + _padding;
            }
        }
        CCLOG("_pad pos:%0.1f,%0.1f",_pad->getPosition().x,_pad->getPosition().y);
	}

    void TouchUpInside(CCObject *senderz, CCControlEvent controlEvent) {
        //setColor(ccc3(255,0,0));
        //setOpacity(100);
        HelloWorld* gameScene = (HelloWorld*)getParent();

        if(gameScene && gameScene->isPaused()) {
            return;
        }


        _lastNode = senderz;

        CCNode* node = (CCNode*) senderz;
		int number = node->getTag();
        gameScene->setValue(number);
    }

	CCSize getSize() {
		return _pad->getContentSize();
	}

	CCSprite* _pad;
    Grid* _pLastGrid;
    CCObject* _lastNode;
    float _padding;
};
InputPad* g_pad;


CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}


void HelloWorld::didAccelerate(CCAcceleration* pAccelerationValue)
{
	static float prevX=0, prevY=0;

#define kFilterFactor 0.05f

	float accelX = (float) pAccelerationValue->x * kFilterFactor + (1- kFilterFactor)*prevX;
	float accelY = (float) pAccelerationValue->y * kFilterFactor + (1- kFilterFactor)*prevY;

	prevX = accelX;
	prevY = accelY;

	CCPoint v = ccp( accelX, accelY);
	v = ccpMult(v, 200);
	m_pSpace->gravity = cpv(v.x, v.y);
}

void HelloWorld::addProgressBar(CCPoint o, CCSize vs) {
    do {
        
        CCSprite *s2=CCSprite::create("Images/progress_bar_bg.png");//pa2.png是较暗的图片 
	    CC_BREAK_IF(!s2);
        //s2->setContentSize(CCSize(200,s2->getContentSize ().height));

        CCPoint progress_pos = ccp(o.x+20,o.y+100 - s2->getContentSize ().height/2);
        s2->setAnchorPoint(ccp(0,0));
        s2->setPosition(progress_pos); 
	    addChild(s2,Z_TOP_LAYER);

	    CCSprite *s=CCSprite::create("Images/progress_bar.png");//pa1.png是较亮的图片
	    CC_BREAK_IF(!s);
        //s->setContentSize(CCSize(200,s2->getContentSize ().height));

	    CCProgressTimer* pt=CCProgressTimer::create(s);
	    CC_BREAK_IF(!pt);
        pt->setAnchorPoint(ccp(0,0));
	    pt->setPercentage(0);
	    pt->setPosition(progress_pos);
        
	    //转圈的CD实现,其实还有个CCProgressTo配合回调实现自动滚动进度
	    //pt->setType(cocos2d::CCProgressTimerType(kCCProgressTimerTypeRadial));  
	    //从中间到外的出现  
	    pt->setType(cocos2d::CCProgressTimerType(kCCProgressTimerTypeBar));
        pt->setBarChangeRate(ccp(1,0));
        pt->setMidpoint(ccp(0,1));
        //第三个参数是tag，也就是node 标记，可以在后面通过标记来访问该对象
	    addChild(pt,Z_TOP_LAYER,kTagProgresBar); 
    } while(0);
}

void HelloWorld::addProgress(CCPoint origin, CCSize visibleSize) {
    CCSprite* time = CCSprite::createWithSpriteFrameName(s_time);
    time->setPosition(ccp(origin.x+time->getContentSize().width/2+10, origin.y + visibleSize.height-time->getContentSize().height/2));
    addChild(time,Z_TOP_LAYER);
    // CCLabelBMFont
    CCLabelBMFont* labelProgress = CCLabelBMFont::create("Time", "fonts/futura-48.fnt");
    labelProgress->setColor(ccc3(0,0,0));
    labelProgress->setScale(0.5f);
    //addChild(labelProgress, Z_TOP_LAYER);
    CCSize sizeTime = time->getContentSize();
    CCLog("size of Time:%f,%f",sizeTime.width,sizeTime.height);
    labelProgress->setPosition(ccp(origin.x+sizeTime.width/2+10, origin.y + visibleSize.height-sizeTime.height/2));

    //时间指示，按秒跳动
    _timeCost0 = loadTimeCost();
    char buff[32];
    int minute = _timeCost0/60;
    int second = (int)_timeCost0%60;
    sprintf(buff, "%02d:%02d", minute,second);
    _labelTimer0 = CCLabelBMFont::create(buff, "fonts/futura-48.fnt");
    _labelTimer0->setColor(ccc3(0,0,0));
    _labelTimer0->setScale(0.5f);
    CCSize sizeTimer = _labelTimer0->getContentSize();

    //添加进度条
    //addProgressBar(origin, visibleSize);

    _labelTimer0->setPosition(ccp(origin.x+sizeTime.width+sizeTimer.width/2-20, origin.y+visibleSize.height - sizeTime.height/2));
    addChild(_labelTimer0,Z_TOP_LAYER);
    
    //每帧调用当前对象的update函数
    //scheduleUpdate();
    //按照指定的间隔启动周期定时器，并调用对应的函数
    schedule(schedule_selector(HelloWorld::timeProcess), 1.0f);
}

void HelloWorld::loadTexture() {
    addImageToSpriteCache(s_nextLevel);
    addImageToSpriteCache(s_time);
    addImageToSpriteCache(s_continue);
    addImageToSpriteCache(s_continue_sel);
    addImageToSpriteCache(s_InputPad);
    addImageToSpriteCache(s_grid);
    addImageToSpriteCache(s_grid_frame);
    addImageToSpriteCache(s_pause);
    addImageToSpriteCache(s_play);
    addImageToSpriteCache(s_pause_sel);
    addImageToSpriteCache(s_play_sel);
    addImageToSpriteCache(s_home);
    addImageToSpriteCache(s_home_sel);
    addImageToSpriteCache(s_reveal);
    addImageToSpriteCache(s_reveal_sel);
    addImageToSpriteCache(s_gameBoard);
    addImageToSpriteCache(s_circle);
    
    
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("Images/ccbDefaultImages.plist");

    CCTextureCache::sharedTextureCache()->addImage("fx/light.png");
}

void HelloWorld::initMenu(CCPoint origin, CCSize visibleSize) {

	CCMenuItemImage *pBackItem = CCMenuItemImage::create(s_back,
										s_back_sel,
										this,menu_selector(HelloWorld::menuGoBackMenu));
    float scaleValue = 1.0f;
	pBackItem->setScale(scaleValue);
   
    //开关式的按钮需要封装一下，否则加起来太麻烦了
    CCScale9Sprite* pauseSprite1 = CCScale9Sprite::createWithSpriteFrameName(s_pause);
    CCScale9Sprite* pauseSprite2 = CCScale9Sprite::createWithSpriteFrameName(s_pause_sel);
    CCMenuItemSprite* pauseItem = CCMenuItemSprite::create(pauseSprite1,pauseSprite2,this,NULL);
    
    CCScale9Sprite* continueSprite1 = CCScale9Sprite::createWithSpriteFrameName(s_continue);
    CCScale9Sprite* continueSprite2 = CCScale9Sprite::createWithSpriteFrameName(s_continue_sel);
    CCMenuItemSprite* continueItem = CCMenuItemSprite::create(continueSprite1,continueSprite2,this,NULL);
     
    CCMenuItemToggle* pauseToggle = CCMenuItemToggle::createWithTarget(this,
        menu_selector(HelloWorld::menuPauseCallback),pauseItem,continueItem,NULL);

    pauseToggle->setScale(scaleValue);
   
    CCMenuItemImage *homeItem = CCMenuItemImage::create(s_home, s_home_sel,
									this,menu_selector(HelloWorld::menuGotoMainMenu));
	homeItem->setScale(scaleValue);

    //CCMenuItemImage* revealItem = CCMenuItemImage::create(s_reveal,s_reveal_sel,
    //    this,menu_selector(HelloWorld::menuRevealPressed));
    //revealItem->setScale(scaleValue*0.5f);

    CCMenuItemImage* resetItem = CCMenuItemImage::create(s_reset,s_reset,
        this,menu_selector(HelloWorld::menuResetPressed));
   
	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pBackItem, pauseToggle,homeItem, NULL);
    CCLOG("menu x:%0.1f backItem x:%0.1f",pMenu->getPosition().x , pBackItem->getPositionX());
    pMenu->setContentSize(homeItem->getContentSize());
    CCLOG("menu width:%0.1f",pMenu->getContentSize().width);
    pMenu->setAnchorPoint(CCPoint(0.5,0.5));
	pMenu->setPosition(visibleSize.width/2,homeItem->getContentSize().height*scaleValue/2);
    pMenu->alignItemsHorizontallyWithPadding(-20);
	addChild(pMenu, 1 , kTagGameSceneMenu);
    CCLOG("menu x:%0.1f backItem x:%0.1f",pMenu->getPosition().x , pBackItem->getPositionX());
}

void HelloWorld::initPauseWindow(CCPoint origin) {
    float padding = 2;

    //初始化暂停界面
    _pauseWindow = CCScale9Sprite::create(s_pauseBkg);
    _pauseWindow->setContentSize(CCSizeMake((g_gridSize.width+padding)*SUDOKU_GRID,
        (g_gridSize.height+padding)*SUDOKU_GRID));
    _pauseWindow->setAnchorPoint(ccp(0.0,0.0));
    _pauseWindow->setPosition(origin);
    _pauseWindow->setVisible(false);
    addChild(_pauseWindow,Z_TOP_LAYER);
}

void HelloWorld::initGameBoard(CCPoint origin, CCSize visibleSize) {
	
    float padding = 2;
	CCLOG("grid size width:%0.1f,height:%0.1f",g_gridSize.width,g_gridSize.height);

    float boardHeight = 0;
    float boarderSize = 4;
	XAXA::LevelMap* level_map = XAXA::LevelMgr::instance()->get_level_map(XAXA::LevelMgr::instance()->get_curr_level());
	XAXA::SudokuLevelMap* sudoku = dynamic_cast<XAXA::SudokuLevelMap*>(level_map);
	int* level = sudoku->get_level();
	for(int i=0;i<SUDOKU_GRID;i++) {
		for(int j=0;j<SUDOKU_GRID;j++) {
			bool isStatic = (level[i*SUDOKU_GRID+j] !=255);
			Grid* pGrid = Grid::GridWithTexture(CCTextureCache::sharedTextureCache()->textureForKey(s_grid),level[i*SUDOKU_GRID+j],isStatic);
            float x = origin.x+boarderSize+j*(g_gridSize.width+padding)+padding*j/3;
            float y = origin.y+boarderSize+i*(g_gridSize.height+padding)+padding*i/3;
            boardHeight = y;
            //CCLOG("grid(%d,%d) pos:%0.1f,%0.1f",j,i,x,y);
			pGrid->GetNode()->setPosition(ccp(x,y));
            pGrid->GetNode()->setAnchorPoint(ccp(0,0));
            pGrid->setLayout(i,j);
            if(255 != loadInput(j,i)) {
                pGrid->setValue(loadInput(j,i));
            }
            
            pGrid->setRevealed(loadRevealer(j,i));

			this->addChild(pGrid->GetNode(), Z_GRID_NUMBER);
			g_GridList[i*SUDOKU_GRID+j] = pGrid;
		}
	}

    boardHeight = (boardHeight - origin.y) + g_gridSize.height + boarderSize;
    CCScale9Sprite* gameBoard = CCScale9Sprite::createWithSpriteFrameName(s_gameBoard);
    //gameBoard->setAnchorPoint(ccp(0.5,0.5));
    gameBoard->setPosition(visibleSize.width/2,origin.y + boardHeight/2);
    addChild(gameBoard,Z_GRID_NUMBER-1);
}

void HelloWorld::initRevealBtn(CCPoint origin, CCSize visibleSize) {
    CCControlButton* revealBtn = createButtonWithTitle("",s_reveal,s_reveal_sel,ccc3(0,0,0),20);
    revealBtn->setPreferredSize(CCSize(64,64));
    //revealBtn->setScale(0.8);
    revealBtn->addTargetWithActionForControlEvents(this,
        cccontrol_selector(HelloWorld::menuRevealPressed), CCControlEventTouchUpInside);  
    revealBtn->setAnchorPoint(ccp(0,1));
    revealBtn->setPosition(visibleSize.width/2,visibleSize.height);
    addChild(revealBtn,Z_HUD_LAYER);
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

    _isWin = false;
    _isPaused = false;
	_inputGrid = NULL;

	//此处开启单点触摸,需要重写ccTouchBegan，ccTouchEnded
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true); 
	//此处开启多点触摸
	//this->setTouchEnabled(true);

    //预加载纹理资源
    loadTexture();

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCLOG("visible(%0.1f,%0.1f), origin(%f,%f)",visibleSize.width,visibleSize.height ,origin.x,origin.y);

    //添加计时进度
    addProgress(origin,visibleSize);

    initRevealBtn(origin,visibleSize);

	//g_logger->Log(WStrToUTF8(L"你好").c_str());

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.
    initMenu(origin,visibleSize);

	/////////////////////////////
	// 3. add your codes below...

	_background = CCSprite::create(s_BackGround);
	_background->setVisible(true);

	// position the sprite on the center of the screen
	_background->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(_background, Z_BACKGROUND);

	int i = 0;
    float padding = 2;

	CCPoint gridOrigin(ccp(O_GRID_X,O_GRID_Y));

    CCTexture2D* gridTexture = CCTextureCache::sharedTextureCache()->addImage(s_grid);
	g_gridSize = gridTexture->getContentSize();

    initPauseWindow(gridOrigin);

    // add the pad
	g_pad = InputPad::create();
    //cclayer这里设置锚点好像没有意义，是因为他本身没有内容吧
	//g_pad->setAnchorPoint(ccp(0.5,0.5));
	//g_pad->setVisible(false);
    CCNode* pMenu = getChildByTag(kTagGameSceneMenu);
    float menuHeight = pMenu->getContentSize().height;
    float padHeight = g_pad->getSize().height;
    CCLOG("menuHeight:%0.1f,padHeight:%0.1f",menuHeight,padHeight);
	//g_pad->setPosition(ccp(visibleSize.width /2 + origin.x, menuHeight + origin.y));
    g_pad->setPosition(0,menuHeight + origin.y);
    CCLOG("pad pos: %0.1f,%0.1f",g_pad->getPosition().x,g_pad->getPosition().y);
	this->addChild(g_pad,Z_INPUTPAD);


    initGameBoard(gridOrigin, visibleSize);

	return true;
}

void HelloWorld::timeProcess(float dt) {
    //胜利后或者暂停后都不计时
    if(_isWin || _isPaused) {
        return;
    }
    _timeCost0 +=dt;

    char str[10] = {0};
    int minute = _timeCost0/60;
    int second = (int)_timeCost0%60;
    sprintf(str, "%02d:%02d", minute,second);
    CCLOG("HelloWorld::timeProcess() time cost:%0.1f",_timeCost0);
    _labelTimer0->setString( str ); 

    CCProgressTimer* pt=(CCProgressTimer*)getChildByTag(kTagProgresBar);
    if(pt) {
        pt->setPercentage(100.0*_timeCost0/10);
    }
}

//调用ScheduleUpdate后，每帧都会调用此函数，基本等于60fps
void HelloWorld::update(float dt) {
    static float time_cost = 0.0f;
    time_cost += dt;
    CCLOG("HelloWorld::update() time cost:%2.6f",time_cost);
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HelloWorld::menuGoBackMenu(CCObject* pSender) {
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
    saveTimeCost();
    REPLACE_SCENE_FROM_CCBI(LevelLayer);
	//这里必须取消触摸代理，否则由于被引用将导致无法释放
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}


void HelloWorld::menuGotoMainMenu(CCObject* pSender) {
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
    saveTimeCost();
    REPLACE_SCENE_FROM_CCBI(MainLayer);
	//这里必须取消触摸代理，否则由于被引用将导致无法释放
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void HelloWorld::menuPauseCallback(CCObject* pSender) {
    if(_isWin) {
        SimpleAudioEngine::sharedEngine()->playEffect(FAIL_FILE);
        return;
    }
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);

    if(_isPaused) {
        //此处开启单点触摸,需要重写ccTouchBegan，ccTouchEnded
	    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);
        _pauseWindow->setVisible(false);
        _isPaused = false;
        return;
    }

    //此处关闭单点触摸,需要重写ccTouchBegan，ccTouchEnded
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    _pauseWindow->setVisible(true);
    _isPaused = true;
    WapsAd::showAd(0);
}
void HelloWorld::menuRevealPressed(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    
    if(_isWin || !_inputGrid) {
        SimpleAudioEngine::sharedEngine()->playEffect(FAIL_FILE); 
        return;
    }

	XAXA::LevelMap* level_map = XAXA::LevelMgr::instance()->get_level_map(XAXA::LevelMgr::instance()->get_curr_level());
	XAXA::SudokuLevelMap* sudoku = dynamic_cast<XAXA::SudokuLevelMap*>(level_map);
	int* level = sudoku->get_level_win();

    //进入照妖镜状态，可以直接将当前选中的框中的数字显示出来，不需要再输入了
    //Grid* pLastGrid = g_pad->getLastGrid();
    if(_inputGrid) {
        if(_inputGrid->isRevealed() || _inputGrid->IsStatic()) {
            SimpleAudioEngine::sharedEngine()->playEffect(FAIL_FILE);   
            return;
        }

        SimpleAudioEngine::sharedEngine()->playEffect(sound_cast_reveal);

        //增加一个爆炸的效果
        //setColor(ccBLACK);
        //removeChild(m_background, true);
        //m_background = NULL;

        CCParticleSystemQuad* emitter = new CCParticleSystemQuad();
        std::string filename = "Particles/ExplodingRing.plist";
        emitter->initWithFile(filename.c_str());
        addChild(emitter, Z_TOP_LAYER);
        emitter->setPosition(_inputGrid->GetNode()->getPosition());
        CCPoint layout = _inputGrid->getLayout();
        int value = level[(int)layout.y*SUDOKU_GRID+(int)layout.x];
        _inputGrid->setRevealed(true);
        saveRevealer(_inputGrid);
        _inputGrid->setValue(value);
        afterEnterValue(_inputGrid);
    }
}

void HelloWorld::menuGoNextLevelPressed(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    
    if(!_isWin) {
        SimpleAudioEngine::sharedEngine()->playEffect(FAIL_FILE); 
        return;
    }

	unsigned int sn = XAXA::LevelMgr::instance()->get_curr_level();
    if(sn+1 ==XAXA::LevelMgr::instance()->get_level_count()) {
        SimpleAudioEngine::sharedEngine()->playEffect(FAIL_FILE); 
        return;
    }
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
    XAXA::LevelMgr::instance()->set_curr_level(sn+1);
    CCScene* se=HelloWorld::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, se));
}

void HelloWorld::menuResetPressed(CCObject* pSender) {
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
}

//要记录的游戏内容：
//1、最后一次的游戏时间，记录的时候要加1秒，避免通过反复启动游戏来获取较短的游戏时间，潜在的bug
//2、游戏时已经填写的数字，包括手工输入的和使用照妖镜
//3、游戏解锁
//4、游戏分数,有吗？
//5、游戏胜利时的时间，取最短的
//6、游戏中使用的照妖镜个数
//7、游戏剩余的照妖镜个数，默认有3个，可以配置，也可以在游戏中获取

void HelloWorld::saveLock() {
    char buff[32];
    LEVEL_SN_TYPE level = XAXA::LevelMgr::instance()->get_curr_level();

    sprintf(buff,"level_%d_lock",(int)(level+1));
    ScoreMgr::instance()->saveInt(buff, (int)0);
}

void HelloWorld::saveGameWin() {
    char buff[32];
    sprintf(buff,"level_%d_win",(int)XAXA::LevelMgr::instance()->get_curr_level());
    ScoreMgr::instance()->saveInt(buff, (int)1);
}

void HelloWorld::saveTimeCost() {
    char buff[32];
    LEVEL_SN_TYPE level = XAXA::LevelMgr::instance()->get_curr_level();
    sprintf(buff,"level_%d_timecost",(int)level);
    CCLOG("saveTimeCost() %s %f",buff, _timeCost0);
    ScoreMgr::instance()->saveInt(buff,(int)((_timeCost0-1) > 0? (_timeCost0-1):0));
}

int HelloWorld::loadTimeCost() {
    char buff[32];
    LEVEL_SN_TYPE level = XAXA::LevelMgr::instance()->get_curr_level();
    sprintf(buff,"level_%d_timecost",(int)level);
    int timeCost = ScoreMgr::instance()->loadInt(buff);
    CCLOG("loadTimeCost() %s %d",buff, timeCost);
    return timeCost;
}

void HelloWorld::clearInput() {
    char buff[32];
    LEVEL_SN_TYPE level = XAXA::LevelMgr::instance()->get_curr_level();

    for(int row = 0; row < SUDOKU_GRID; row++) {
        for(int col = 0; col < SUDOKU_GRID; col++) {
            sprintf(buff,"level_%d_grid_%d_%d",(int)level,col,row);
            //CCLOG("saveInput() %s %d",buff, 255);
            ScoreMgr::instance()->saveInt(buff,255);

            sprintf(buff,"level_%d_revealer_%d_%d",(int)level,col,row);
            CCLOG("saveRevealer() %s",buff);
            ScoreMgr::instance()->saveInt(buff, (int)0);
        }
    }

    CCLOG("clearInput()");
}


int HelloWorld::loadInput(int col, int row) {
    char buff[32];
    LEVEL_SN_TYPE level = XAXA::LevelMgr::instance()->get_curr_level();
    sprintf(buff,"level_%d_grid_%d_%d",(int)level,col,row);

    int value = ScoreMgr::instance()->loadInt(buff,255);
    CCLOG("loadInput() %s %d",buff, value);
    return value;
}

void HelloWorld::saveInput(int col, int row, int value) {
    char buff[32];
    LEVEL_SN_TYPE level = XAXA::LevelMgr::instance()->get_curr_level();
    sprintf(buff,"level_%d_grid_%d_%d",(int)level,col,row);

    CCLOG("saveInput() %s %d",buff, value);
    ScoreMgr::instance()->saveInt(buff,value);
}

int HelloWorld::loadRevealer(int col, int row) {
    char buff[32];
    LEVEL_SN_TYPE level = XAXA::LevelMgr::instance()->get_curr_level();
    sprintf(buff,"level_%d_revealer_%d_%d",(int)level,col,row);

    int value = ScoreMgr::instance()->loadInt(buff);
    CCLOG("loadRevealer() %s %d",buff, value);
    return value;
}

void HelloWorld::saveRevealer(Grid* grid){
    char buff[32];
    LEVEL_SN_TYPE level = XAXA::LevelMgr::instance()->get_curr_level();
    CCPoint pos = grid->getLayout();
    sprintf(buff,"level_%d_revealer_%d_%d",(int)level,(int)pos.x,(int)pos.y);
    CCLOG("saveRevealer() %s",buff);
    ScoreMgr::instance()->saveInt(buff, (int)1);
}

void HelloWorld::afterEnterValue(Grid* grid) {
    CCPoint pos = grid->getLayout();
    saveInput((int)pos.x,(int)pos.y,grid->getValue());
    if(checkWin()) {
	    //此处关闭单点触摸,只能点击按钮进行下一步
	    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this); 
	    showWin();
        unlockNextLevel();
        //将胜利信息存入
        _isWin = true;
        saveGameWin();
        //关卡通关后输入记录和时间都清零
        clearInput();
        _timeCost0 = 0;
        saveTimeCost();
    }
}

bool HelloWorld::canInput(CCTouch* pTouch) {
	Grid* pGrid = getGrid(pTouch);
	if(pGrid) {
		return !pGrid->IsStatic() || !pGrid->isRevealed();
	}
	
	return false;
}

Grid* HelloWorld::getGrid(CCTouch* pTouch) {
    float padding = 2;
	CCPoint gridOrigin(ccp(O_GRID_X,O_GRID_Y));
	CCPoint loc = pTouch->getLocation();
    CCLOG("loc:%0.1f,%0.1f",loc.x,loc.y);
    float width = loc.x - gridOrigin.x;
	int i = (width)/(g_gridSize.width+padding);
    float offset = width - (g_gridSize.width+padding)*i;
    CCLOG("width:%0.1f,offset:%0.1f,i:%d",width,offset,i);
    if((offset < padding) || (offset > (g_gridSize.width+padding)))
    {
        return NULL;
    }

    float height = loc.y - gridOrigin.y;
	int j = (height)/(g_gridSize.height+padding);
    offset = height - (g_gridSize.height+padding)*j;
    CCLOG("height:%0.1f,offset:%0.1f,j:%d",height,offset,j);
    if((offset < padding) || (offset > (g_gridSize.height+padding)))
    {
        return NULL;
    }

	if(i < 0 || j < 0 || i >= SUDOKU_GRID || j >= SUDOKU_GRID) {
		return NULL;
	}
	
	return g_GridList[j*SUDOKU_GRID+i];
}

bool HelloWorld::checkWin() {
	XAXA::LevelMap* level_map = XAXA::LevelMgr::instance()->get_level_map(XAXA::LevelMgr::instance()->get_curr_level());
	XAXA::SudokuLevelMap* sudoku = dynamic_cast<XAXA::SudokuLevelMap*>(level_map);
	int* level = sudoku->get_level_win();
    //先按规则进行比较
    //1、已经填满了每个格子
    for (int i = 0; i < SUDOKU_GRID; i++)
    {
        for (int j = 0; j <SUDOKU_GRID; j++)
        {
            if(g_GridList[i*SUDOKU_GRID+j]->getValue() == 255) {
                return false;
            }
        }
    }

    //2、每行不重复
    for (int i = 0; i < SUDOKU_GRID; i++)
    {
        std::map<int,int> test;
        int tmp = 0;
        for (int j = 0; j <SUDOKU_GRID; j++)
        {
            tmp = g_GridList[i*SUDOKU_GRID+j]->getValue();
            if(tmp != 255 && test.count(tmp) >0) {
                return false;
            }
            test.insert(std::make_pair(tmp,1));
        }
    }
    //3、每列不重复
    for (int i = 0; i < SUDOKU_GRID; i++)
    {
        std::map<int,int> test;
        int tmp = 255;
        for (int j = 0; j <SUDOKU_GRID; j++)
        {
            tmp = g_GridList[j*SUDOKU_GRID+i]->getValue();
            if(tmp != 255 && test.count(tmp) >0) {
                return false;
            }
            test.insert(std::make_pair(tmp,1));
        }
    }
    //4、小9宫格中不重复
    for (int i = 0; i < SUDOKU_GRID/3; i++)
    {
        for (int j = 0; j < SUDOKU_GRID/3; j++)
        {
            std::map<int,int> test;
            int tmp = 255;
            for (int k = 0; k < SUDOKU_GRID/3; k++)
            {
                for (int l = 0; l < SUDOKU_GRID/3; l++)
                {
                    tmp = g_GridList[(i*3+k)*SUDOKU_GRID+j*3+l]->getValue();
                    if(tmp != 255 && test.count(tmp) >0) {
                        return false;
                    }
                    test.insert(std::make_pair(tmp,1));
                }
            }
        }
    }

    return true;

    //直接跟默认的结果比较，如果如果有多结局的，可能出现问题
	for(int i=0;i<SUDOKU_GRID;i++) {
		for(int j=0;j<SUDOKU_GRID;j++) {
			if(g_GridList[i*SUDOKU_GRID+j]->getValue() != level[i*SUDOKU_GRID+j]) {
				return false;
			}
		}
	}

	return true;
}

void HelloWorld::showWin() {
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    //播放奖励音效
    SimpleAudioEngine::sharedEngine()->playEffect(sound_victory);
    //出现奖励徽章
    CCSprite* badge = CCSprite::create(s_badge);
    addChild(badge,Z_TOP_LAYER);
    badge->setPosition(CCPoint(visibleSize.width/2,visibleSize.height*5/9));
    badge->setScale(1.7f);
    CCActionInterval* scaleTo = CCScaleTo::create(0.2f,1.0f);
    CCActionInterval*  actionBack1 = CCScaleTo::create(0.1f,1.1f);
    CCActionInterval*  actionBack2 = CCScaleTo::create(0.1f,1.0f);
    CCActionInterval*  actionBack3 = CCScaleTo::create(0.05f,1.03f);
    CCActionInterval*  actionBack4 = CCScaleTo::create(0.05f,1.0f);

    CCControlButton * btnNextLevel = createButtonWithTitle("",s_nextLevel,s_nextLevel);
    btnNextLevel->setPreferredSize(CCSize(64,32));
    btnNextLevel->addTargetWithActionForControlEvents(this,
        cccontrol_selector(HelloWorld::menuRevealPressed), CCControlEventTouchUpInside); 
    btnNextLevel->setAnchorPoint(ccp(0,1));
    btnNextLevel->setPosition(visibleSize.width/2,visibleSize.height);

    badge->addChild(btnNextLevel,Z_HUD_LAYER);
	//添加一些动作，可以仅仅是单个动作，也可以是多个动作组合
	badge->runAction( CCSequence::create(scaleTo, actionBack1, actionBack2, actionBack3,actionBack4, NULL));

    CCParticleSystem* m_emitter = new CCParticleSystemQuad();
    m_emitter->initWithTotalParticles(50);
    //m_emitter->autorelease();

    addChild(m_emitter, Z_TOP_LAYER+1);
    ////m_emitter->release();    // win32 :  use this line or remove this line and use autorelease()
    m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage(s_stars1) );

    m_emitter->setDuration(2);
    
    // gravity
    m_emitter->setGravity(CCPointZero);
    
    // angle
    m_emitter->setAngle(90);
    m_emitter->setAngleVar(360);
    
    // speed of particles
    m_emitter->setSpeed(160);
    m_emitter->setSpeedVar(20);
    
    // radial
    m_emitter->setRadialAccel(-120);
    m_emitter->setRadialAccelVar(0);
    
    // tagential
    m_emitter->setTangentialAccel(30);
    m_emitter->setTangentialAccelVar(0);
    
    // emitter position
    m_emitter->setPosition( ccp(160,240) );
    m_emitter->setPosVar(CCPointZero);
    
    // life of particles
    m_emitter->setLife(2);
    m_emitter->setLifeVar(0);
    
    // spin of particles
    m_emitter->setStartSpin(0);
    m_emitter->setStartSizeVar(0);
    m_emitter->setEndSpin(0);
    m_emitter->setEndSpinVar(0);
    
    // color of particles
    ccColor4F startColor = {0.5f, 0.5f, 0.5f, 1.0f};
    m_emitter->setStartColor(startColor);
    
    ccColor4F startColorVar = {0.5f, 0.5f, 0.5f, 1.0f};
    m_emitter->setStartColorVar(startColorVar);
    
    ccColor4F endColor = {0.1f, 0.1f, 0.1f, 0.2f};
    m_emitter->setEndColor(endColor);
    
    ccColor4F endColorVar = {0.1f, 0.1f, 0.1f, 0.2f};    
    m_emitter->setEndColorVar(endColorVar);
    
    // size, in pixels
    m_emitter->setStartSize(80.0f);
    m_emitter->setStartSizeVar(40.0f);
    m_emitter->setEndSize(kParticleStartSizeEqualToEndSize);
    
    // emits per second
    m_emitter->setEmissionRate(m_emitter->getTotalParticles()/m_emitter->getLife());
    
    // additive
    m_emitter->setBlendAdditive(true);

	//m_emitter->retain();
	
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	if (m_emitter != NULL)
	{
		m_emitter->setPosition( ccp(s.width / 2, s.height / 2) );
	}
}

bool HelloWorld::highLightGrid(CCTouch *pTouch) {
	Grid* pGrid = getGrid(pTouch);
	if(pGrid) {
        Grid* pLastGrid = _inputGrid;
        if(pLastGrid) {
            pLastGrid->hilite(false);
        }
        SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
		pGrid->hilite(true);
        _inputGrid = pGrid;

        return true;
	}

    return false;
}

void HelloWorld::unlockNextLevel() {
    unsigned int level_sn = XAXA::LevelMgr::instance()->get_curr_level();

    if(level_sn+1 == XAXA::LevelMgr::instance()->get_level_count()) {
        return;
    }
    XAXA::LevelInfo* levelInfo = XAXA::LevelMgr::instance()->get_level_info(level_sn+1);
    if(levelInfo) {
        levelInfo->is_lock = false;
        saveLock();
        
    }
}

void HelloWorld::setValue(int number) {
    if(_isWin || !_inputGrid || _inputGrid->isRevealed() || _inputGrid->IsStatic()) {
        SimpleAudioEngine::sharedEngine()->playEffect(FAIL_FILE); 
        return;
    }
    SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE); 

    _inputGrid->setValue(number);
    afterEnterValue(_inputGrid);
}

//如果是简单的单击处理，只需要在Began中处理就可以了，无需到ccTouchEnded中
bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    /*
	//如果输入盘打开了，并且点中了输入盘的某个数字，则填入选择的数字,并隐藏输入盘
	if(g_pad->isVisible() && g_pad->containsTouchLocation(pTouch)) {
		int number = g_pad->getNumber(pTouch);
		_inputGrid->setValue(number);
		g_pad->setVisible(false);
        afterEnterValue(_inputGrid);
		return true;
	}*/

    //高亮选中的方格
    return highLightGrid(pTouch);

    /*
	//如果是初始的数字，不允许修改
	if(!canInput(pTouch)) {
		g_pad->setVisible(false);
		return true;
	}*/

	//CCPoint pos = pTouch->getLocation();

	//pos.x = getBound(pos.x,g_pad->rect().getMaxX(),0,_background->getContentSize().width);
	//pos.y = getBound(pos.y,g_pad->rect().getMaxY(),0,_background->getContentSize().height);

	//CCLOG("final pos x:%0.1f,y:%0.1f",pos.x,pos.y);

	//runFx(pos);
	
	//g_pad->setPosition(pos);
    /*
	g_pad->setScale(0.1f);
	g_pad->setVisible(true);
	SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
	CCActionInterval*  actionTo = CCScaleTo::create(0.05f, 1.2f);
	CCActionInterval*  actionBack = CCScaleTo::create(0.05f,1.0f);
	//添加一些动作，可以仅仅是单个动作，也可以是多个动作组合
	//g_pad->runAction(actionTo);
	g_pad->runAction( CCSequence::create(actionTo, actionBack, NULL));
	_inputGrid = getGrid(pTouch);
	*/
	return true;
}
void HelloWorld::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
}

void HelloWorld::runFx(cocos2d::CCPoint point) {
	CCTextureCache::sharedTextureCache()->addImage("fx/light.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("fx/light.plist",CCTextureCache::sharedTextureCache()->textureForKey("fx/light.png"));
	CCArray* sperci=CCArray::create();
	
	for(int i=0;i<18;i++){
	sperci->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s%d.png","light",i)->getCString()));
	}
	CCSprite* monst=CCSprite::createWithSpriteFrame((CCSpriteFrame*)sperci->objectAtIndex(0));

	CCAnimation *animation=CCAnimation::createWithSpriteFrames(sperci,0.05f);
	CCAnimate *animate=CCAnimate::create(animation);
	CCCallFuncN *onComplete =  CCCallFuncN::create(this, callfuncN_selector(HelloWorld::runFxCallBack));  
	CCSequence* pse=CCSequence::create(animate,onComplete,NULL);
	monst->setAnchorPoint(getAnchorPoint());
	point.y +=60;
	monst->setPosition(point);
	addChild(monst,100);
	
	//monst->runAction(CCRepeatForever::create(pse));
	monst->setScale(0.5f);
	monst->runAction(pse);
}

void HelloWorld::runFxCallBack(cocos2d::CCNode* pSend){
	CCSprite* monst=(CCSprite*)pSend;
	CCNode* de= monst->getParent();
	de->removeChild(pSend,true);
	//de->stopSpeciaCollisionMonster();
}


}
