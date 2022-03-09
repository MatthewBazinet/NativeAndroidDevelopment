LOCAL_PATH := $(call my-dir)
PROJECT_ROOT_PATH := $(LOCAL_PATH)/../../../
CORE_RELATIVE_PATH := ../../../core/
 
include $(CLEAR_VARS)
 
LOCAL_MODULE    := game
LOCAL_CFLAGS    := -Wall -Wextra
LOCAL_SRC_FILES := platform_asset_utils.c \
                   platform_log.c \
                   $(CORE_RELATIVE_PATH)/asset_utils.c \
                   $(CORE_RELATIVE_PATH)/buffer.c \
                   $(CORE_RELATIVE_PATH)/OBJLoader.cpp \
                   $(CORE_RELATIVE_PATH)/shader.c \
                   $(CORE_RELATIVE_PATH)/texture.c \
                   
LOCAL_LDLIBS := -lGLESv2 -llog -landroid
 
include $(BUILD_SHARED_LIBRARY)
 
$(call import-add-path,$(PROJECT_ROOT_PATH)/3rdparty)
