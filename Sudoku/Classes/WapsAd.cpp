#include "WapsAd.h"

WapsAd::WapsAd(){}
WapsAd::~WapsAd(){}

void WapsAd::showAd(int adTag)
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //�жϵ�ǰ�Ƿ�ΪAndroidƽ̨
    JniMethodInfo showAd;
  
    bool isHave = JniHelper::getStaticMethodInfo(showAd,"com/test/sudoku/sudoku","showAdStatic", "(I)V"); 

    if (!isHave) {
        CCLog("jni:showAdStatic is null");
    }else{
        //���ô˺���
        showAd.env->CallStaticVoidMethod(showAd.classID, showAd.methodID,adTag);
    }
    #endif

}

	
