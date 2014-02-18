
#include "actions\CCActionInterval.h"
#include "chipmunk.h"

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include <stdio.h>
#include <string.h>

#include <list>

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
		_pValueLabel = CCLabelAtlas::create("",s_FpsImage,16,20,'.');
		setValue(value);
		if(_isStatic) {
			_pValueLabel->setColor(ccc3(100,200,255));
		}

		if(pTexture) {
			//_pGrid = CCSprite::createWithTexture(pTexture);
            _pHighlight = CCScale9Sprite::createWithSpriteFrameName("button_hi.png");
            _pHighlight->setContentSize(g_gridSize);
            _pHighlight->setAnchorPoint(ccp(0.5f,0.5f));
			_pHighlight->setPosition(g_gridSize.width/2,g_gridSize.height/2);
            _pHighlight->setVisible(false);

			//CCSize s = _pValueLabel->getContentSize();
			//CCLOG("label width:%0.1f,height:%0.1f",s.width,s.height);
			_pValueLabel->setAnchorPoint(ccp(0.5f,0.5f));
			_pValueLabel->setPosition(g_gridSize.width/2,g_gridSize.height/2);

            _pGrid = CCScale9Sprite::createWithSpriteFrameName("ccbButton.png");
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

	static Grid* GridWithTexture(CCTexture2D* aTexture,int value=255, bool isStatic=false);

private:
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
class InputPad: public CCLayer{
public:
	CREATE_FUNC(InputPad);

	InputPad() {
        _pLastGrid = NULL;
		_pad = CCSprite::create(s_InputPad);
        _pad->setOpacity(128);
		this->addChild(_pad,Z_INPUTPAD);

		CCTexture2D* gridTexture = CCTextureCache::sharedTextureCache()->addImage(s_GridUp);
		CCSize s = gridTexture->getContentSize();
		CCPoint gridOrigin(ccp(0,0));
	
		for(int i=0;i<ROW_NUM;i++) {
			for(int j=0;j<COL_NUM;j++) {
				Grid* pGrid = Grid::GridWithTexture(gridTexture,(i*COL_NUM+j+1),true);
				float x = gridOrigin.x+j*(g_gridSize.width+1)+1;
				float y = gridOrigin.y+i*(g_gridSize.height+1)+1;
				//CCLOG("input grid:x:%0.1f, y:%0.1f",x,y);
				pGrid->GetNode()->setPosition(ccp(x,y));
				pGrid->GetNode()->setAnchorPoint(ccp(0.0f,0.0f));
				_pad->addChild(pGrid->GetNode(), Z_INPUT_GRID_NUMBER);
			}
		}

		//CCLayerColor* pBackGround = CCLayerColor::create(ccc4(255,255,0,255),90,90);
		//this->addChild(pBackGround,0);
	}

    void setGrid(Grid* pGrid) {
        _pLastGrid = pGrid;
    }

    Grid* getLastGrid() {
        return _pLastGrid;
    }
	CCRect rect()
	{
		CCSize s = _pad->getContentSize();
		return CCRectMake(-s.width/2, -s.height/2, s.width, s.height);
	}
	bool containsTouchLocation(CCTouch* touch)
	{
		return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
	}

	int getNumber(CCTouch* touch) {
		CCPoint localPoint = _pad->convertTouchToNodeSpace(touch);
		//CCPoint localPoint = touch->getLocation();
		CCSize s = _pad->getContentSize();
		int x = localPoint.x/g_gridSize.width;
		int y = ((int)(localPoint.y/ g_gridSize.height))*3;
		int number =  x+y+1;
		return number;
	}

	CCSize getSize() {
		return _pad->getContentSize();
	}

	CCSprite* _pad;
    Grid* _pLastGrid;
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


void HelloWorld::initPhysic() {
#if CC_ENABLE_CHIPMUNK_INTEGRATION

	//启动重力加速度，系统会定时调用didAccelerate
	setAccelerometerEnabled(true);

	m_pSpace = cpSpaceNew();

	m_pSpace->gravity = cpv(0, -100);

	//
	// rogue shapes
	// We have to free them manually
	//
	// bottom
	m_pWalls[0] = cpSegmentShapeNew( m_pSpace->staticBody,
		cpv(VisibleRect::leftBottom().x,VisibleRect::leftBottom().y),
		cpv(VisibleRect::rightBottom().x, VisibleRect::rightBottom().y), 0.0f);

	// top
	m_pWalls[1] = cpSegmentShapeNew( m_pSpace->staticBody, 
		cpv(VisibleRect::leftTop().x, VisibleRect::leftTop().y),
		cpv(VisibleRect::rightTop().x, VisibleRect::rightTop().y), 0.0f);

	// left
	m_pWalls[2] = cpSegmentShapeNew( m_pSpace->staticBody,
		cpv(VisibleRect::leftBottom().x,VisibleRect::leftBottom().y),
		cpv(VisibleRect::leftTop().x,VisibleRect::leftTop().y), 0.0f);

	// right
	m_pWalls[3] = cpSegmentShapeNew( m_pSpace->staticBody, 
		cpv(VisibleRect::rightBottom().x, VisibleRect::rightBottom().y),
		cpv(VisibleRect::rightTop().x, VisibleRect::rightTop().y), 0.0f);

	for( int i=0;i<4;i++) {
		m_pWalls[i]->e = 1.0f;
		m_pWalls[i]->u = 1.0f;
		cpSpaceAddStaticShape(m_pSpace, m_pWalls[i] );
	}

	// Physics debug layer
	m_pDebugLayer = CCPhysicsDebugNode::create(m_pSpace);
	this->addChild(m_pDebugLayer, Z_PHYSICS_DEBUG);
#endif
}

void HelloWorld::addPhysicSprite() {
#if CC_ENABLE_CHIPMUNK_INTEGRATION
	// Use batch node. Faster
	CCSpriteBatchNode *parent = CCSpriteBatchNode::create(s_SpinPea, 100);
	m_pSpriteTexture = parent->getTexture();

	addChild(parent, 100, kTagParentNode);

	CCPoint pos = ccp(200,200);
	int posx, posy;

	CCNode *parent = getChildByTag(kTagParentNode);

	posx = CCRANDOM_0_1() * 200.0f;
	posy = CCRANDOM_0_1() * 200.0f;

	posx = (posx % 4) * 85;
	posy = (posy % 3) * 121;


	int num = 4;
	cpVect verts[] = {
		cpv(-24,-54),
		cpv(-24, 54),
		cpv( 24, 54),
		cpv( 24,-54),
	};

	cpBody *body = cpBodyNew(1.0f, cpMomentForPoly(1.0f, num, verts, cpvzero));

	body->p = cpv(pos.x, pos.y);
	cpSpaceAddBody(m_pSpace, body);

	cpShape* shape = cpPolyShapeNew(body, num, verts, cpvzero);
	shape->e = 0.5f; shape->u = 0.5f;
	cpSpaceAddShape(m_pSpace, shape);

	CCPhysicsSprite *sprite = CCPhysicsSprite::createWithTexture(m_pSpriteTexture, CCRectMake(posx, posy, 85, 121));
	parent->addChild(sprite,50);

	sprite->setCPBody(body);
	sprite->setPosition(pos);
#endif
}

/*
void init9sprite() {
	int space = 10; // px
		
	double max_w = 0, max_h = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// Add the buttons
			CCControlButton *button = standardButtonWithTitle(CCString::createWithFormat("%d",rand() % 30)->getCString());
			button->setAdjustBackgroundImage(false);  // Tells the button that the background image must not be adjust
												// It'll use the prefered size of the background image
			button->setPosition(ccp (button->getContentSize().width / 2 + (button->getContentSize().width + space) * i,
									button->getContentSize().height / 2 + (button->getContentSize().height + space) * j));
			layer->addChild(button);
				
			max_w = MAX(button->getContentSize().width * (i + 1) + space  * i, max_w);
			max_h = MAX(button->getContentSize().height * (j + 1) + space * j, max_h);
		}
	}
	CCScale9Sprite *backgroundButton = CCScale9Sprite::create("extensions/buttonBackground.png");
	backgroundButton->setContentSize(CCSizeMake(max_w + 14, max_h + 14));
	backgroundButton->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
	addChild(backgroundButton);
}
*/

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
    // CCLabelBMFont
    CCLabelBMFont *_labelProgress = CCLabelBMFont::create("Time:", "fonts/futura-48.fnt");
    _labelProgress->setScale(0.5f);
    addChild(_labelProgress, Z_TOP_LAYER);
    CCSize sizeTime = _labelProgress->getContentSize();
    CCLog("size of Time:%f,%f",sizeTime.width,sizeTime.height);
    _labelProgress->setPosition(ccp(origin.x+sizeTime.width/2+10, origin.y + visibleSize.height-sizeTime.height/2));

    //时间指示，按秒跳动
    _timeCost0 = 0.0f;
    _labelTimer0 = CCLabelBMFont::create("0", "fonts/futura-48.fnt");
    _labelTimer0->setScale(0.5f);
    _labelTimer0->setColor(ccc3(255,0,0));
    CCSize sizeTimer = _labelProgress->getContentSize();

    //添加进度条
    addProgressBar(origin, visibleSize);

    _labelTimer0->setPosition(ccp(origin.x+sizeTime.width+sizeTimer.width/2+5, origin.y+visibleSize.height - sizeTime.height/2));
    addChild(_labelTimer0,Z_TOP_LAYER);
    
    //每帧调用当前对象的update函数
    //scheduleUpdate();
    //按照指定的间隔启动周期定时器，并调用对应的函数
    schedule(schedule_selector(HelloWorld::timeProcess), 1.0f);
}

void HelloWorld::loadTexture() {
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("Images/ccbDefaultImages.plist");

    CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage("Images/button_hi.png");
    CCSpriteFrame* pSpriteFrame = CCSpriteFrame::createWithTexture(pTexture,CCRect(0,0,pTexture->getContentSize().width,pTexture->getContentSize ().height ));
    cache->addSpriteFrame(pSpriteFrame,"button_hi.png");

    CCTextureCache::sharedTextureCache()->addImage("fx/light.png");
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

    //initPhysic();
	//addPhysicSprite();

	_inputGrid = NULL;

	//此处开启单点触摸,需要重写ccTouchBegan，ccTouchEnded
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true); 
	//此处开启多点触摸
	//this->setTouchEnabled(true);

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCLOG("visible(%0.1f,%0.1f), origin(%f,%f)",visibleSize.width,visibleSize.height ,origin.x,origin.y);

    //添加计时进度
    addProgress(origin,visibleSize);

    //预加载纹理资源
    loadTexture();

	//g_logger->Log(WStrToUTF8(L"你好").c_str());

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
										s_BtnCloseNormal,
										s_BtnCloseSelected,
										this,
										menu_selector(HelloWorld::menuCloseCallback));
	
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width/2 - pCloseItem->getContentSize().width/2 ,
								origin.y + pCloseItem->getContentSize().height/2));

	CCMenuItemFont *pMenuItem = CCMenuItemFont::create("Main",this,menu_selector(HelloWorld::menuGotoMainMenu));
	pMenuItem->setFontSize(10);
	pMenuItem->setColor(ccc3(255,255,255));
	pMenuItem->setPosition(ccp(origin.x + visibleSize.width/2 - pCloseItem->getContentSize().width/2 ,
								origin.y + pCloseItem->getContentSize().height/2+30));
	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pCloseItem,pMenuItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	/////////////////////////////
	// 3. add your codes below...

	_background = CCSprite::create(s_BackGround);
	_background->setVisible(true);

	// position the sprite on the center of the screen
	_background->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(_background, Z_BACKGROUND);

	_background->setScale(1.0);

	int i = 0;
	CCTexture2D* gridTexture = CCTextureCache::sharedTextureCache()->addImage(s_Grid);
	g_gridSize = gridTexture->getContentSize();

	CCLOG("grid size width:%0.1f,height:%0.1f",g_gridSize.width,g_gridSize.height);
	CCPoint gridOrigin(ccp(O_GRID_X,O_GRID_Y));
	
	g_pad = InputPad::create();
	g_pad->setAnchorPoint(ccp(0,0));
	g_pad->setVisible(false);
	g_pad->setPosition(ccp(visibleSize.width /2 + origin.x, g_pad->getContentSize ().height/2 + origin.y));
	this->addChild(g_pad,Z_INPUTPAD);

	CCLOG("g_pad width:%0.1f,height:%0.1f",g_pad->getContentSize().width,g_pad->getContentSize().height);

	XAXA::LevelMap* level_map = XAXA::LevelMgr::instance()->get_level_map(XAXA::LevelMgr::instance()->get_curr_level());
	XAXA::SudokuLevelMap* sudoku = dynamic_cast<XAXA::SudokuLevelMap*>(level_map);
	int* level = sudoku->get_level();
	for(i=0;i<SUDOKU_GRID;i++) {
		for(int j=0;j<SUDOKU_GRID;j++) {
			bool isStatic = (level[i*SUDOKU_GRID+j] !=255);
			Grid* pGrid = Grid::GridWithTexture(gridTexture,level[i*SUDOKU_GRID+j],isStatic);
			pGrid->GetNode()->setPosition(ccp(gridOrigin.x+j*(g_gridSize.width+2),
				gridOrigin.y+(i+1)*(g_gridSize.height+2)));
			this->addChild(pGrid->GetNode(), Z_GRID_NUMBER);
			g_GridList[i*SUDOKU_GRID+j] = pGrid;
		}
	}

	return true;
}

void HelloWorld::timeProcess(float dt) {
    _timeCost0 +=dt;

    char str[10] = {0};
    sprintf(str, "%d", (int)_timeCost0);
    CCLOG("HelloWorld::timeProcess() time cost:%0.1f",_timeCost0);
    _labelTimer0->setString( str ); 

    CCProgressTimer* pt=(CCProgressTimer*)getChildByTag(kTagProgresBar);
    pt->setPercentage(100.0*_timeCost0/10);
}

//调用ScheduleUpdate后，每帧都会调用此函数，基本等于60fps
void HelloWorld::update(float dt) {
    static float time_cost = 0.0f;
    time_cost += dt;
    CCLOG("HelloWorld::update() time cost:%2.6f",time_cost);
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HelloWorld::saveScores() {
    //我们这里简单存储条数据
    LEVEL_SN_TYPE level = XAXA::LevelMgr::instance()->get_curr_level();
    ScoreMgr::instance()->saveInt("level", (int)level);
    ScoreMgr::instance()->saveInt("timeCost",(int)_timeCost0);
    
    //取出我们刚存储的然后赋值给str验证下；
    int testReadLevel= ScoreMgr::instance()->loadInt("level");
    int testReadTimeCost = ScoreMgr::instance()->loadInt("timeCost");
    CCLog("level:%d,time:%d",testReadLevel,testReadTimeCost);

}

void HelloWorld::menuGotoMainMenu(CCObject* pSender) {
    saveScores();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.2f, MainLayer::scene()));
	//这里必须取消触摸代理，否则由于被引用将导致无法释放
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

}

bool HelloWorld::canInput(CCTouch* pTouch) {
	Grid* pGrid = getGrid(pTouch);
	if(pGrid) {
		return !pGrid->IsStatic();
	}
	
	return false;
}

Grid* HelloWorld::getGrid(CCTouch* pTouch) {
	CCPoint gridOrigin(ccp(O_GRID_X,O_GRID_Y));
	CCPoint loc = pTouch->getLocation();
	int i = (loc.x - gridOrigin.x)/g_gridSize.width;
	int j = (loc.y - gridOrigin.y)/g_gridSize.height-1;
	if(i < 0 || j < 0 || i >= SUDOKU_GRID || j >= SUDOKU_GRID) {
		return NULL;
	}
	
	return g_GridList[j*SUDOKU_GRID+i];
}

bool HelloWorld::checkWin() {
	XAXA::LevelMap* level_map = XAXA::LevelMgr::instance()->get_level_map(XAXA::LevelMgr::instance()->get_curr_level());
	XAXA::SudokuLevelMap* sudoku = dynamic_cast<XAXA::SudokuLevelMap*>(level_map);
	int* level = sudoku->get_level_win();

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

	CCParticleSystem* m_emitter= CCParticleFireworks::create();
	m_emitter->retain();
	this->addChild(m_emitter, Z_WIN_FIREWORK);
	
	m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage(s_stars1) );
	
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	if (m_emitter != NULL)
	{
		m_emitter->setPosition( ccp(s.width / 2, s.height / 2) );
	}
}

void HelloWorld::highLightGrid(CCTouch *pTouch) {
	Grid* pGrid = getGrid(pTouch);
	if(pGrid) {
        Grid* pLastGrid = g_pad->getLastGrid();
        if(pLastGrid) {
            pLastGrid->hilite(false);
        }
		pGrid->hilite(true);
        g_pad->setGrid(pGrid);
	}
}

//如果是简单的单击处理，只需要在Began中处理就可以了，无需到ccTouchEnded中
bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {

	//如果输入盘打开了，并且点中了输入盘的某个数字，则填入选择的数字,并隐藏输入盘
	if(g_pad->isVisible() && g_pad->containsTouchLocation(pTouch)) {
		int number = g_pad->getNumber(pTouch);
		_inputGrid->setValue(number);
		g_pad->setVisible(false);
		if(checkWin()) {
			//此处关闭单点触摸,只能点击按钮进行下一步
			CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this); 
			showWin();
		}
		return true;
	}

    //高亮选中的方格
    highLightGrid(pTouch);

	//如果是初始的数字，不允许修改
	if(!canInput(pTouch)) {
		g_pad->setVisible(false);
		return true;
	}

	CCPoint pos = pTouch->getLocation();

	pos.x = getBound(pos.x,g_pad->rect().getMaxX(),0,_background->getContentSize().width);
	pos.y = getBound(pos.y,g_pad->rect().getMaxY(),0,_background->getContentSize().height);

	//CCLOG("final pos x:%0.1f,y:%0.1f",pos.x,pos.y);

	//runFx(pos);
	
	//g_pad->setPosition(pos);
	g_pad->setScale(0.1f);
	g_pad->setVisible(true);
	SimpleAudioEngine::sharedEngine()->playEffect(CLICK_FILE);
	CCActionInterval*  actionTo = CCScaleTo::create(0.05f, 1.2f);
	CCActionInterval*  actionBack = CCScaleTo::create(0.05f,1.0f);
	//添加一些动作，可以仅仅是单个动作，也可以是多个动作组合
	//g_pad->runAction(actionTo);
	g_pad->runAction( CCSequence::create(actionTo, actionBack, NULL));
	_inputGrid = getGrid(pTouch);
	
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
