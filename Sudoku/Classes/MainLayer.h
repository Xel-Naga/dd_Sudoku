#ifndef __MAIN_LAYER_H__
#define __MAIN_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

namespace XAXA {

class MainLayer : public CCLayer
{
public:
    static CCScene* scene() {
	    // 'scene' is an autorelease object
	    CCScene *scene = CCScene::create();
	
	    // 'layer' is an autorelease object
	    CCLayer *layer = MainLayer::create();

	    // add layer as a child to scene
	    scene->addChild(layer);

	    // return the scene
	    return scene;
    }

    MainLayer() { CCLog("MainLayer::Ctor()");}
    ~MainLayer() { CCLog("MainLayer::Dtor()");}


    bool init();

    // implement the "static node()" method manually
    //create方法是静态函数，必须手工生成
    CREATE_FUNC(MainLayer);

private:
    void onNewGame(CCObject* pSender);

    void onSettings(CCObject* pSender);

    void onAbout(CCObject* pSender);

    void initSound();

    void initLevel();
};

}
#endif

