LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ninjadash_shared

LOCAL_MODULE_FILENAME := libninjadashcpp

LOCAL_SRC_FILES := ninjadash/main.cpp \
				   ../../Classes/AppDelegate.cpp \
                   ../../Classes/GameRunScene.cpp \
                   ../../proj.win32/GameOverScene.cpp \
                   ../../proj.win32/GameMenuScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../proj.win32

LOCAL_WHOLE_STATIC_LIBRARIES := cocos_ninjadash_common
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
#LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
#LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
#LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
#LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
