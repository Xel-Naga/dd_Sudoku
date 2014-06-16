#ifndef _UTILS_H_
#define _UTILS_H_
#include <string>

typedef unsigned char uint8;
typedef unsigned short uint16;

//将Unicode小端转为UTF8，主要用于中文输出
std::string UCS2ToUTF8(const std::string& src);
int UTF8ToUCS2(const uint8 *utf8Str, uint16 *ucs2Buf, int ucs2MaxLen);
std::string WStrToUTF8(const std::wstring& str);

//获取边界，用于当坐标超过屏幕左边界或者右边界时自动对齐边界值，
//是的对象显示不会超出屏幕
float getBound(float x,float offset,float min,float max);

#define CREATE_AUTORELEASE_LOADER(layer_name) \
class layer_name##Loader: public cocos2d::extension::CCLayerLoader { \
    public: \
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(layer_name##Loader, loader); \
    protected: \
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(layer_name);  \
};


#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;


#define REPLACE_SCENE_FROM_CCBI(layer_name) \
{ \
    /* Create an autorelease CCNodeLoaderLibrary. */ \
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); \
    ccNodeLoaderLibrary->registerCCNodeLoader(#layer_name, layer_name##Loader::loader()); \
    /* Create an autorelease CCBReader. */ \
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); \
    /*非常重要，这里ccb根目录设置不正确会导致图片加载失败，找不到对应的文件*/ \
    ccbReader->setCCBRootPath(""); \
    /* Read a ccbi file. */ \
    CCNode * node = ccbReader->readNodeGraphFromFile(#layer_name".ccbi", this); \
    ccbReader->release(); \
    CCScene * scene = CCScene::create(); \
    if(node != NULL) { \
        scene->addChild(node); \
    } \
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f,scene)); \
    CCLOG(#layer_name":: reference:%d",this->retainCount()); \
}

CCControlButton * createButtonWithTitle(const char * title, const char* bg, const char* bg_hi,ccColor3B color=ccc3(0,0,0),float fontSize=20);

void addImageToSpriteCache(const char* imageName);

#endif