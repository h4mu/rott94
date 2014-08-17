LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c
LOCAL_SRC_FILES += ../../../rott/cin_actr.c
LOCAL_SRC_FILES += ../../../rott/cin_efct.c
LOCAL_SRC_FILES += ../../../rott/cin_evnt.c
LOCAL_SRC_FILES += ../../../rott/cin_glob.c
LOCAL_SRC_FILES += ../../../rott/cin_main.c
LOCAL_SRC_FILES += ../../../rott/cin_util.c
LOCAL_SRC_FILES += ../../../rott/dosutil.c
LOCAL_SRC_FILES += ../../../rott/engine.c
LOCAL_SRC_FILES += ../../../rott/isr.c
LOCAL_SRC_FILES += ../../../rott/modexlib.c
LOCAL_SRC_FILES += ../../../rott/rt_actor.c
LOCAL_SRC_FILES += ../../../rott/rt_battl.c
LOCAL_SRC_FILES += ../../../rott/rt_build.c
LOCAL_SRC_FILES += ../../../rott/rt_cfg.c
LOCAL_SRC_FILES += ../../../rott/rt_crc.c
LOCAL_SRC_FILES += ../../../rott/rt_com.c
LOCAL_SRC_FILES += ../../../rott/rt_debug.c
LOCAL_SRC_FILES += ../../../rott/rt_dmand.c
LOCAL_SRC_FILES += ../../../rott/rt_door.c
LOCAL_SRC_FILES += ../../../rott/rt_draw.c
LOCAL_SRC_FILES += ../../../rott/rt_floor.c
LOCAL_SRC_FILES += ../../../rott/rt_game.c
LOCAL_SRC_FILES += ../../../rott/rt_in.c
LOCAL_SRC_FILES += ../../../rott/rt_main.c
LOCAL_SRC_FILES += ../../../rott/rt_map.c
LOCAL_SRC_FILES += ../../../rott/rt_menu.c
LOCAL_SRC_FILES += ../../../rott/rt_msg.c
LOCAL_SRC_FILES += ../../../rott/rt_net.c
LOCAL_SRC_FILES += ../../../rott/rt_playr.c
LOCAL_SRC_FILES += ../../../rott/rt_rand.c
LOCAL_SRC_FILES += ../../../rott/rt_scale.c
LOCAL_SRC_FILES += ../../../rott/rt_sound.c
LOCAL_SRC_FILES += ../../../rott/rt_spbal.c
LOCAL_SRC_FILES += ../../../rott/rt_sqrt.c
LOCAL_SRC_FILES += ../../../rott/rt_stat.c
LOCAL_SRC_FILES += ../../../rott/rt_state.c
LOCAL_SRC_FILES += ../../../rott/rt_str.c
LOCAL_SRC_FILES += ../../../rott/rt_swift.c
LOCAL_SRC_FILES += ../../../rott/rt_ted.c
LOCAL_SRC_FILES += ../../../rott/rt_util.c
LOCAL_SRC_FILES += ../../../rott/rt_view.c
LOCAL_SRC_FILES += ../../../rott/rt_vid.c
LOCAL_SRC_FILES += ../../../rott/rt_err.c
LOCAL_SRC_FILES += ../../../rott/scriplib.c
LOCAL_SRC_FILES += ../../../rott/w_wad.c
LOCAL_SRC_FILES += ../../../rott/watcom.c
LOCAL_SRC_FILES += ../../../rott/z_zone.c
LOCAL_SRC_FILES += ../../../rott/byteordr.c
LOCAL_SRC_FILES += ../../../rott/dukemusc.c
LOCAL_SRC_FILES += ../../../rott/winrott.c
LOCAL_SRC_FILES += ../../../rott/audiolib/fx_man.c
LOCAL_SRC_FILES += ../../../rott/audiolib/dsl.c
LOCAL_SRC_FILES += ../../../rott/audiolib/ll_man.c
LOCAL_SRC_FILES += ../../../rott/audiolib/multivoc.c
LOCAL_SRC_FILES += ../../../rott/audiolib/mv_mix.c
LOCAL_SRC_FILES += ../../../rott/audiolib/mvreverb.c
LOCAL_SRC_FILES += ../../../rott/audiolib/nodpmi.c
LOCAL_SRC_FILES += ../../../rott/audiolib/pitch.c
LOCAL_SRC_FILES += ../../../rott/audiolib/user.c
LOCAL_SRC_FILES += ../../../rott/audiolib/usrhooks.c

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_mixer

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

APP_OPTIM := debug

LOCAL_CFLAGS += -g -ggdb -O0 -DSHAREWARE=0 -DSUPERROTT=0 -DSITELICENSE=1 -DUSE_SDL=1 -DPLATFORM_UNIX=1 -DANDROID=1 -DS_IREAD=S_IRUSR -DS_IWRITE=S_IWUSR -DS_IEXEC=S_IXUSR

include $(BUILD_SHARED_LIBRARY)
