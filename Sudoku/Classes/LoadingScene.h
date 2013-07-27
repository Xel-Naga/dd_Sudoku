#ifndef _LOADINGSCENE_H_
#define _LOADINGSCENE_H_

#include "cocos2d.h"

namespace XAXA {

class LoadingScene: public cocos2d::CCLayer {
public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(LoadingScene);

private:
    bool prepareLoading();
    void goWelcomeScene();
    void loadCallBack(CCObject* ped);

    int _loadingNum;
    int _totalNum;
};


}//namespace XAXA

#endif