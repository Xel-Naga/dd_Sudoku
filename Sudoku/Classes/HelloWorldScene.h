#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

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

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    bool canInput(CCTouch* pTouch);
    Grid* getGrid(CCTouch* pTouch);

    bool checkWin();
    void showWin();

    void initPhysic();
    void addPhysicSprite();

private:
    Grid* _inputGrid;
    CCSprite* _background;
    cpSpace* m_pSpace; // strong ref
    cpShape* m_pWalls[4];
    CCTexture2D* m_pSpriteTexture; // weak ref
#if CC_ENABLE_CHIPMUNK_INTEGRATION    
    CCPhysicsDebugNode* m_pDebugLayer; // weak ref
#endif
};

}
#endif // __HELLOWORLD_SCENE_H__
