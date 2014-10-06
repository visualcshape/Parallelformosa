LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := sqlite3_static

LOCAL_MODULE_FILENAME := sqlite3

LOCAL_SRC_FILES := \
include/sqlite3.c


LOCAL_EXPORT_C_INCLUDES :=$(LOCAL_PATH)/include \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

include $(BUILD_STATIC_LIBRARY)

