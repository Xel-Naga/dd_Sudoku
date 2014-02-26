LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/LevelLayer.cpp \
                   ../../Classes/LevelMgr.cpp \
                   ../../Classes/LoadingScene.cpp \
                   ../../Classes/MainLayer.cpp \
                   ../../Classes/Utils.cpp \
		   ../../Classes/ResolutionAdaptor.cpp \
		   ../../Classes/ScoreMgr.cpp \
		   ../../Classes/WapsAd.cpp
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/jsoncpp \
		    $(LOCAL_PATH)/../../../../bagua_engine/include \
		    

#LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocos_extension_static jsoncpp_static
LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocos_extension_static jsoncpp_static          
include $(BUILD_SHARED_LIBRARY)

$(call import-module,jsoncpp) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)

