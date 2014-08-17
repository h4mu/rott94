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
# \
#	 cin_actr.c cin_efct.c cin_evnt.c cin_glob.c cin_main.c cin_util.c dosutil.c engine.c isr.c modexlib.c rt_actor.c rt_battl.c rt_build.c rt_cfg.c rt_crc.c rt_com.c rt_debug.c rt_dmand.c rt_door.c rt_draw.c rt_floor.c rt_game.c rt_in.c rt_main.c rt_map.c rt_menu.c rt_msg.c rt_net.c rt_playr.c rt_rand.c rt_scale.c rt_sound.c rt_spbal.c rt_sqrt.c rt_stat.c rt_state.c rt_str.c rt_swift.c rt_ted.c rt_util.c rt_view.c rt_vid.c rt_err.c scriplib.c w_wad.c watcom.c z_zone.c byteordr.c dukemusc.c winrott.c fx_man.c dsl.c ll_man.c multivoc.c mv_mix.c mvreverb.c nodpmi.c pitch.c user.c usrhooks.c
	 
#	 byteordr.c byteordr.h cin_actr.c cin_actr.h cin_def.h cin_efct.c cin_efct.h cin_evnt.c cin_evnt.h cin_glob.c cin_glob.h cin_main.c cin_main.h cin_util.c cin_util.h develop.h dosutil.c dukemusc.c engine.c _engine.h engine.h fli_def.h fli_glob.h fli_main.c fli_main.h fli_type.h fli_util.c fli_util.h f_scale.h fx_man.c fx_man.h gmove.h isr.c _isr.h isr.h keyb.h lookups.c lumpy.h memcheck.h modexlib.c modexlib.h music.h myprint.h profile.h rottnet.h _rt_acto.h rt_actor.c rt_actor.h rt_battl.c rt_battl.h rt_build.c rt_build.h _rt_buil.h rt_cfg.c rt_cfg.h rt_com.c _rt_com.h rt_com.h rt_crc.c rt_crc.h rt_debug.c rt_debug.h rt_def.h rt_dmand.c rt_dmand.h _rt_dman.h rt_door.c _rt_door.h rt_door.h rt_dr_a.h rt_draw.c _rt_draw.h rt_draw.h rt_err.c rt_error.c rt_error.h rt_fc_a.h _rt_floo.h rt_floor.c rt_floor.h rt_game.c _rt_game.h rt_game.h rt_in.c _rt_in.h rt_in.h rt_main.c _rt_main.h rt_main.h rt_map.c _rt_map.h rt_map.h rt_menu.c _rt_menu.h rt_menu.h rt_msg.c _rt_msg.h rt_msg.h rt_net.c _rt_net.h rt_net.h _rt_play.h rt_playr.c rt_playr.h rt_rand.c _rt_rand.h rt_rand.h rt_sc_a.h rt_scale.c rt_scale.h _rt_scal.h rt_sound.c rt_sound.h _rt_soun.h _rt_spba.h rt_spbal.c rt_spbal.h rt_sqrt.c rt_sqrt.h rt_stat.c rt_state.c _rt_stat.h rt_stat.h rt_str.c _rt_str.h rt_str.h _rt_swft.h rt_swift.c rt_swift.h rt_table.h rt_ted.c _rt_ted.h rt_ted.h rt_util.c _rt_util.h rt_util.h rt_vh_a.h rt_vid.c _rt_vid.h rt_vid.h rt_view.c rt_view.h sbconfig.c sbconfig.h scriplib.c scriplib.h sndcards.h snd_reg.h snd_shar.h splib.h sprites.h states.h task_man.h version.h watcom.c watcom.h winrott.c WinRott.h w_wad.c _w_wad.h w_wad.h z_zone.c _z_zone.h z_zone.h adlibfx.c adlibfx.h al_midi.c _al_midi.h al_midi.h assert.h awe32.c awe32.h blaster.c _blaster.h blaster.h ctaweapi.h debugio.c debugio.h dma.c dma.h dpmi.c dpmi.h dsl.c dsl.h fx_man.c fx_man.h gmtimbre.c gus.c gusmidi.c gusmidi.h guswave.c _guswave.h guswave.h interrup.h irq.c irq.h leetimbr.c linklist.h ll_man.c ll_man.h midi.c _midi.h midi.h mpu401.c mpu401.h _multivc.h multivoc.c multivoc.h music.c music.h mv_mix.c mvreverb.c myprint.c myprint.h newgf1.h nodpmi.c nomusic.c pas16.c _pas16.h pas16.h pitch.c pitch.h platform.h sndcards.h sndscape.c sndscape.h _sndscap.h sndsrc.c sndsrc.h standard.h task_man.c task_man.h user.c user.h usrhooks.c usrhooks.h util.h


LOCAL_SHARED_LIBRARIES := SDL2 SDL2_mixer

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

APP_OPTIM := debug

LOCAL_CFLAGS += -g -ggdb -O0 -DSHAREWARE=0 -DSUPERROTT=0 -DSITELICENSE=1 -DUSE_SDL=1 -DPLATFORM_UNIX=1 -DANDROID=1 -DS_IREAD=S_IRUSR -DS_IWRITE=S_IWUSR -DS_IEXEC=S_IXUSR

include $(BUILD_SHARED_LIBRARY)
