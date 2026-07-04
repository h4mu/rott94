# Dreamcast Makefile for ROTT

# You must source your KOS environ script before running this.

ROTT_BASE = ../rott

SRCS = cin_actr.c cin_efct.c cin_evnt.c cin_glob.c cin_main.c cin_util.c \
       dosutil.c engine.c isr.c modexlib.c rt_actor.c rt_battl.c \
       rt_build.c rt_cfg.c rt_crc.c rt_com.c rt_debug.c rt_dmand.c \
       rt_door.c rt_draw.c rt_floor.c rt_game.c rt_in.c rt_main.c \
       rt_map.c rt_menu.c rt_msg.c rt_net.c rt_playr.c rt_rand.c \
       rt_scale.c rt_sound.c rt_spbal.c rt_sqrt.c rt_stat.c rt_state.c \
       rt_str.c rt_swift.c rt_ted.c rt_util.c rt_view.c rt_vid.c \
       rt_err.c scriplib.c w_wad.c watcom.c z_zone.c byteordr.c \
       dukemusc.c winrott.c

# Audiolib sources
AUDIO_SRCS = adlibfx.c al_midi.c debugio.c dma.c dpmi.c dsl.c fx_man.c \
             ll_man.c midi.c multivoc.c music.c mv_mix.c mvreverb.c \
             myprint.c pitch.c task_man.c user.c usrhooks.c

VPATH = $(ROTT_BASE):$(ROTT_BASE)/audiolib

KOS_CFLAGS += -I$(ROTT_BASE) -I$(ROTT_BASE)/audiolib -I$(KOS_BASE)/addons/include/SDL2
KOS_CFLAGS += -DPLATFORM_DREAMCAST=1 -DUSE_SDL=1

# Link with SDL2 and SDL2_mixer
KOS_LIBS += -lSDL2_mixer -lSDL2 -lz -lm

all: rott-huntbgin.elf rott-darkwar.elf rott-rottcd.elf rott-rottsite.elf

rott-huntbgin.elf: $(SRCS:%.c=huntbgin/%.o) $(AUDIO_SRCS:%.c=huntbgin/%.o)
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $@ $^ $(KOS_LIBS)

rott-darkwar.elf: $(SRCS:%.c=darkwar/%.o) $(AUDIO_SRCS:%.c=darkwar/%.o)
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $@ $^ $(KOS_LIBS)

rott-rottcd.elf: $(SRCS:%.c=rottcd/%.o) $(AUDIO_SRCS:%.c=rottcd/%.o)
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $@ $^ $(KOS_LIBS)

rott-rottsite.elf: $(SRCS:%.c=rottsite/%.o) $(AUDIO_SRCS:%.c=rottsite/%.o)
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $@ $^ $(KOS_LIBS)

huntbgin/%.o: %.c
	@mkdir -p huntbgin
	$(KOS_CC) $(KOS_CFLAGS) -DSHAREWARE=1 -DSUPERROTT=0 -DSITELICENSE=0 -c $< -o $@

darkwar/%.o: %.c
	@mkdir -p darkwar
	$(KOS_CC) $(KOS_CFLAGS) -DSHAREWARE=0 -DSUPERROTT=0 -DSITELICENSE=0 -c $< -o $@

rottcd/%.o: %.c
	@mkdir -p rottcd
	$(KOS_CC) $(KOS_CFLAGS) -DSHAREWARE=0 -DSUPERROTT=1 -DSITELICENSE=0 -c $< -o $@

rottsite/%.o: %.c
	@mkdir -p rottsite
	$(KOS_CC) $(KOS_CFLAGS) -DSHAREWARE=0 -DSUPERROTT=0 -DSITELICENSE=1 -c $< -o $@

clean:
	rm -rf huntbgin darkwar rottcd rottsite *.elf
