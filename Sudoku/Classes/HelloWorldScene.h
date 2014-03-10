#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;
#undef CC_ENABLE_CHIPMUNK_INTEGRATION

//namespace cocos2d {
struct cpSpace;
struct cpShape;
//}
namespace XAXA {
class Grid;
class HelloWorld : public cocos2d::CCLayer
{
public:
    HelloWorld() { CCLog("HelloWorld::ctor()");}
    ~HelloWorld() { CCLog("HelloWorld::dtor()");}
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    virtual void didAccelerate(CCAcceleration* pAccelerationValue);

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

private:    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    void menuGotoMainMenu(CCObject* pSender);
    void menuGoBackMenu(CCObject* pSender);
    void menuPauseCallback(CCObject* pSender);

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    bool canInput(CCTouch* pTouch);
    Grid* getGrid(CCTouch* pTouch);

    bool checkWin();
    void showWin();

    void runFx(cocos2d::CCPoint point);
    void runFxCallBack(cocos2d::CCNode* pSend);

    void timeProcess(float dt);
    void update(float dt);

    void saveScores();

    void addProgress(CCPoint origin, CCSize visibleSize);
    void addProgressBar(CCPoint origin, CCSize visibleSize);

    void highLightGrid(CCTouch *pTouch);
    void loadTexture();

    void unlockNextLevel();

    void initMenu(CCSize visibleSize,CCPoint origin);

private:
    CCLabelBMFont*    _labelProgress;
    CCLabelBMFont*    _labelTimer0;
    float   _timeCost0;
    Grid* _inputGrid;
    CCSprite* _background;
    cpSpace* m_pSpace; // strong ref
    cpShape* m_pWalls[4];
    CCTexture2D* m_pSpriteTexture; // weak ref
    bool _isWin;//是否已经过关
    bool _isPaused;//是否暂停
#if CC_ENABLE_CHIPMUNK_INTEGRATION
    CCPhysicsDebugNode* m_pDebugLayer; // weak ref
#endif
};

}
#endif // __HELLOWORLD_SCENE_H__
