#ifndef _UTILS_H_
#define _UTILS_H_
#include <string>

typedef unsigned char uint8;
typedef unsigned short uint16;

//��UnicodeС��תΪUTF8����Ҫ�����������
std::string UCS2ToUTF8(const std::string& src);
int UTF8ToUCS2(const uint8 *utf8Str, uint16 *ucs2Buf, int ucs2MaxLen);
std::string WStrToUTF8(const std::wstring& str);

//��ȡ�߽磬���ڵ����곬����Ļ��߽�����ұ߽�ʱ�Զ�����߽�ֵ��
//�ǵĶ�����ʾ���ᳬ����Ļ
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
    /*�ǳ���Ҫ������ccb��Ŀ¼���ò���ȷ�ᵼ��ͼƬ����ʧ�ܣ��Ҳ�����Ӧ���ļ�*/ \
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