#ifndef __MAIN_LAYER_H__
#define __MAIN_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Utils.h"

USING_NS_CC;

namespace XAXA {

class MainLayer : public CCLayer
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener 
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
    //CREATE_FUNC(MainLayer);
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(MainLayer, create);

    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual void onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);


private:
    void onPressPlay(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);

    void onPressOption(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);

    void onPressExit(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);

    void onPressMore(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);

    void onPressSpeaker(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);

    void initSound();

    void initLevel();

    void saveSpeakerState(bool isOn);

    bool getSpeakerState();

    bool _is_speaker_on;
};

CREATE_AUTORELEASE_LOADER(MainLayer);

}
#endif

