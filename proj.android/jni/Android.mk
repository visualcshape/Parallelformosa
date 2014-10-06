LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/BaseLayer.cpp \
../../Classes/Building.cpp \
../../Classes/CustomLabelMenuItem.cpp \
../../Classes/DataModel.cpp \
../../Classes/Database.cpp \
../../Classes/DialogueWindow.cpp \
../../Classes/DialogueWindowConfirm.cpp \
../../Classes/HUDLayer.cpp \
../../Classes/HelloWorldScene.cpp \
../../Classes/LoginTitleLayer.cpp \
../../Classes/LoginTitleModel.cpp \
../../Classes/MapLayer.cpp \
../../Classes/MapModel.cpp \
../../Classes/Observer.cpp \
../../Classes/ResourceModel.cpp \
../../Classes/SceneManager.cpp \
../../Classes/Subject.cpp \
../../Classes/Tower.cpp \
../../Classes/TrademarkLayer.cpp \
../../Classes/UtilFunc.cpp \
../../Classes/VisibleRect.cpp \

<<<<<<< HEAD
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../cocos2d/cocos \
					$(LOCAL_PATH)/../../cocos2d/external \
					$(LOCAL_PATH)/../../cocos2d/cocos/platform/android/ \
					$(LOCAL_PATH)/../../sqlite3/

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_ui_static

=======

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../cocos2d/cocos/ui \
					$(LOCAL_PATH)/../../libpomelo/include \
					$(LOCAL_PATH)/../../libpomelo/deps/jansson/src \
					$(LOCAL_PATH)/../../libpomelo/deps/uv/include \
					$(LOCAL_PATH)/../../libpomelo/deps/uv/uv-private \
					$(LOCAL_PATH)/../../cocos2d/extensions \
					$(LOCAL_PATH)/../../cocos2d/external/sqlite3/include/

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_ui_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += sqlite3_static
LOCAL_WHOLE_STATIC_LIBRARIES += pomelo_static
>>>>>>> v0.1.3

include $(BUILD_SHARED_LIBRARY)

$(call import-module,libpomelo)
$(call import-module,sqlite3)
$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,Box2D)
$(call import-module,editor-support/cocosbuilder)
$(call import-module,editor-support/spine)
$(call import-module,editor-support/cocostudio)
$(call import-module,network)
$(call import-module,extensions)
$(call import-module,ui)
