#include "WapsAd.h"

WapsAd::WapsAd(){}
WapsAd::~WapsAd(){}

void WapsAd::showAd(int adTag)
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
    JniMethodInfo showAd;
  
    bool isHave = JniHelper::getStaticMethodInfo(showAd,"com/test/sudoku/sudoku","showAdStatic", "(I)V"); 

    if (!isHave) {
        CCLog("jni:showAdStatic is null");
    }else{
        //调用此函数
        showAd.env->CallStaticVoidMethod(showAd.classID, showAd.methodID,adTag);
    }
    #endif

}

	
