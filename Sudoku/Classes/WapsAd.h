#include "platform\CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include "cocos2d.h"
using namespace cocos2d;
#endif

class WapsAd
{
public:
    WapsAd();
    virtual ~WapsAd();
    static void showAd(int adTag);//ͨ��JNI����JAVA��̬������ʵ��չʾAD  
    
};