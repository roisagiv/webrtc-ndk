# Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
#
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file in the root of the source
# tree. An additional intellectual property rights grant can be found
# in the file PATENTS.  All contributing project authors may
# be found in the AUTHORS file in the root of the source tree.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
LOCAL_MODULE_CLASS := STATIC_LIBRARIES
LOCAL_MODULE := libwebrtc_vad
LOCAL_MODULE_TAGS := optional
LOCAL_GENERATED_SOURCES :=
LOCAL_SRC_FILES := webrtc_vad.c \
    vad_const.c \
    vad_core.c \
    vad_filterbank.c \
    vad_gmm.c \
    vad_sp.c

# Flags passed to both C and C++ files.
MY_CFLAGS :=  
MY_CFLAGS_C :=
MY_DEFS := '-DNO_TCMALLOC' \
    '-DNO_HEAPCHECKER' \
    '-DWEBRTC_TARGET_PC' \
    '-DWEBRTC_LINUX' 
ifeq ($(TARGET_ARCH),arm) 
MY_DEFS += \
    '-DWEBRTC_ANDROID' \
    '-DANDROID' 
endif
LOCAL_CFLAGS := $(MY_CFLAGS_C) $(MY_CFLAGS) $(MY_DEFS)

# Include paths placed before CFLAGS/CPPFLAGS
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../.. \
    $(LOCAL_PATH)/../interface \
    $(LOCAL_PATH)/../../../signal_processing_library/main/interface 

# Flags passed to only C++ (and not C) files.
LOCAL_CPPFLAGS := 

LOCAL_LDFLAGS :=

LOCAL_STATIC_LIBRARIES :=

LOCAL_SHARED_LIBRARIES := libdl \
    libstlport
LOCAL_ADDITIONAL_DEPENDENCIES :=

ifeq ($(TARGET_OS)-$(TARGET_SIMULATOR),linux-true)
LOCAL_LDLIBS += -ldl -lpthread
endif

ifneq ($(TARGET_SIMULATOR),true)
LOCAL_SHARED_LIBRARIES += libdl
endif

include external/stlport/libstlport.mk
include $(BUILD_STATIC_LIBRARY)
