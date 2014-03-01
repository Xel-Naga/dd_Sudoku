
#include "LoadingScene.h"
#include "MainLayer.h"
#include "Resource.h"
#include "Utils.h"
USING_NS_CC;

namespace XAXA {

bool LoadingScene::prepareLoading() {
    bool bRet = false;

    _loadingNum = 0;
    _totalNum = 10;//好像这个统计一定要少一个才能正常结束，不知道是啥bug

    do {
        CCSprite *s2=CCSprite::create("Images/Loading_dark.png");//pa2.png是较暗的图片 
	    CC_BREAK_IF(!s2);

	    s2->setPosition(ccp(this->getContentSize().width/2,this->getContentSize().height/2));  
	    addChild(s2,0);

	    CCSprite *s=CCSprite::create("Images/Loading_light.png");//pa1.png是较亮的图片
	    CC_BREAK_IF(!s);

	    CCProgressTimer* pt=CCProgressTimer::create(s);
	    CC_BREAK_IF(!pt);

	    pt->setPercentage(0);

	    pt->setPosition(ccp(this->getContentSize().width/2,this->getContentSize().height/2));  
	    //转圈的CD实现  
	    pt->setType(cocos2d::CCProgressTimerType(kCCProgressTimerTypeRadial));  
	    //从中间到外的出现  
	    //pt->setType(cocos2d::CCProgressTimerType(kCCProgressTimerTypeBar));  
	    this->addChild(pt,1,1); 

        //测试一下中文
        CCLabelTTF* loadingText = CCLabelTTF::create(WStrToUTF8(s_LoadingChs).c_str(),"Arial",30);
        loadingText->setColor(ccc3(247,176,71));
        loadingText->setPosition(ccp(this->getContentSize().width/2,this->getContentSize().height/2-s2->getContentSize().height/2-loadingText->getContentSize().height/2));
        this->addChild(loadingText,2);

        bRet = true;
    } while(0);

    return bRet;
}

bool LoadingScene::init()
{
    bool bRet = false;
	//////////////////////////////
	// 1. super init first
	do {
        CC_BREAK_IF(!CCLayer::init());

        //设置加载界面的进度条
        CC_BREAK_IF(!prepareLoading());

        //开始加载图片
        CCTextureCache::sharedTextureCache()->addImageAsync(s_MainBkg,this,callfuncO_selector(LoadingScene::loadCallBack)); 
        CCTextureCache::sharedTextureCache()->addImageAsync(s_menu,this,callfuncO_selector(LoadingScene::loadCallBack)); 

        CCTextureCache::sharedTextureCache()->addImageAsync(s_BackGround,this,callfuncO_selector(LoadingScene::loadCallBack)); 
 
        CCTextureCache::sharedTextureCache()->addImageAsync(s_Grid,this,callfuncO_selector(LoadingScene::loadCallBack)); 

        CCTextureCache::sharedTextureCache()->addImageAsync(s_GridUp,this,callfuncO_selector(LoadingScene::loadCallBack)); 

        CCTextureCache::sharedTextureCache()->addImageAsync(s_FpsImage,this,callfuncO_selector(LoadingScene::loadCallBack)); 

        CCTextureCache::sharedTextureCache()->addImageAsync(s_InputPad,this,callfuncO_selector(LoadingScene::loadCallBack)); 

        CCTextureCache::sharedTextureCache()->addImageAsync(s_SpinPea,this,callfuncO_selector(LoadingScene::loadCallBack)); 

        CCTextureCache::sharedTextureCache()->addImageAsync(s_BtnCloseNormal,this,callfuncO_selector(LoadingScene::loadCallBack)); 

        CCTextureCache::sharedTextureCache()->addImageAsync(s_BtnCloseSelected,this,callfuncO_selector(LoadingScene::loadCallBack)); 

        bRet = true;

    } while(0);

    return bRet;
}

cocos2d::CCScene* LoadingScene::scene()
{
	// 'scene' is an autorelease object
	cocos2d::CCScene *scene = cocos2d::CCScene::create();
	
	// 'layer' is an autorelease object
	LoadingScene *layer = LoadingScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void LoadingScene::loadCallBack(CCObject* ped){
	_loadingNum++;
    //Sleep(100);
    CCProgressTimer* pt=(CCProgressTimer*)this->getChildByTag(1);
    //for(int i =0; i < (int)(100.0/_totalNum); i++) {
    //    CCLOG("loadCallBack() progress:%d%%",i+(int)now);
    //    pt->setPercentage(i+now);
    //    Sleep(300);
    //}
	pt->setPercentage(100.0*_loadingNum/_totalNum);
    CCLOG("LoadingScene::loadCallBack() loading:%d/%d",_loadingNum,_totalNum);
	if(_loadingNum>=_totalNum){
		pt->setPercentage(100);
		// 加载完的时候跳转到响应的界面
		CCLOG("loading over");
		goWelcomeScene();
	}
}

void LoadingScene::goWelcomeScene() {
    REPLACE_SCENE_FROM_CCBI(MainLayer);
}


}
