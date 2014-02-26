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
    static void showAd(int adTag);//通过JNI调用JAVA静态函数，实现展示AD  
    
};