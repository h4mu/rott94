cmake_minimum_required(VERSION 3.16)

project(rott94_android LANGUAGES C)

include(GNUInstallDirs)

set(SDL3_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL")
set(SDL3_MIXER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL_mixer")

option(ROTT_SHAREWARE "Build the shareware variant" ON)
option(ROTT_SUPERROTT "Build the Super ROTT variant" OFF)
option(ROTT_SITELICENSE "Build the site license variant" OFF)

if(NOT EXISTS "${SDL3_SOURCE_DIR}/CMakeLists.txt")
  message(FATAL_ERROR "SDL3 submodule not found at ${SDL3_SOURCE_DIR}")
endif()
if(NOT EXISTS "${SDL3_MIXER_SOURCE_DIR}/CMakeLists.txt")
  message(FATAL_ERROR "SDL3_mixer submodule not found at ${SDL3_MIXER_SOURCE_DIR}")
endif()

set(SDL_SHARED ON CACHE BOOL "Build SDL shared library" FORCE)
set(SDL_STATIC OFF CACHE BOOL "Build SDL static library" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "Install SDL" FORCE)
set(SDL_UNINSTALL OFF CACHE BOOL "Enable SDL uninstall target" FORCE)
set(SDL_TESTS OFF CACHE BOOL "Build SDL tests" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "Build SDL examples" FORCE)

set(SDLMIXER_VENDORED ON CACHE BOOL "Use SDL_mixer vendored third-party code" FORCE)
set(SDLMIXER_INSTALL OFF CACHE BOOL "Install SDL_mixer" FORCE)
set(SDLMIXER_TESTS OFF CACHE BOOL "Build SDL_mixer tests" FORCE)
set(SDLMIXER_EXAMPLES OFF CACHE BOOL "Build SDL_mixer examples" FORCE)
set(SDLMIXER_WERROR OFF CACHE BOOL "Treat SDL_mixer warnings as errors" FORCE)
set(SDLMIXER_AIFF ON CACHE BOOL "Enable AIFF audio" FORCE)
set(SDLMIXER_WAVE ON CACHE BOOL "Enable WAVE audio" FORCE)
set(SDLMIXER_VOC ON CACHE BOOL "Enable VOC audio" FORCE)
set(SDLMIXER_AU ON CACHE BOOL "Enable AU audio" FORCE)
set(SDLMIXER_FLAC OFF CACHE BOOL "Enable FLAC audio" FORCE)
set(SDLMIXER_GME OFF CACHE BOOL "Enable GME audio" FORCE)
set(SDLMIXER_MOD OFF CACHE BOOL "Enable MOD audio" FORCE)
set(SDLMIXER_MP3 OFF CACHE BOOL "Enable MP3 audio" FORCE)
set(SDLMIXER_MIDI ON CACHE BOOL "Enable MIDI audio" FORCE)
set(SDLMIXER_MIDI_TIMIDITY ON CACHE BOOL "Enable timidity MIDI backend" FORCE)
set(SDLMIXER_OPUS OFF CACHE BOOL "Enable Opus audio" FORCE)
set(SDLMIXER_VORBIS_STB OFF CACHE BOOL "Enable stb_vorbis audio" FORCE)
set(SDLMIXER_VORBIS_VORBISFILE OFF CACHE BOOL "Enable vorbisfile audio" FORCE)
set(SDLMIXER_VORBIS_TREMOR OFF CACHE BOOL "Enable tremor audio" FORCE)
set(SDLMIXER_WAVPACK OFF CACHE BOOL "Enable WavPack audio" FORCE)

if(ROTT_SHAREWARE AND (ROTT_SUPERROTT OR ROTT_SITELICENSE))
  message(FATAL_ERROR "ROTT_SHAREWARE cannot be combined with ROTT_SUPERROTT or ROTT_SITELICENSE")
endif()
if(ROTT_SUPERROTT AND ROTT_SITELICENSE)
  message(FATAL_ERROR "ROTT_SUPERROTT and ROTT_SITELICENSE are mutually exclusive")
endif()

add_subdirectory("${SDL3_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/_deps/SDL3" EXCLUDE_FROM_ALL)
add_subdirectory("${SDL3_MIXER_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/_deps/SDL3_mixer" EXCLUDE_FROM_ALL)

add_library(main SHARED)
target_sources(main PRIVATE
  ../../../../rott/cin_actr.c
  ../../../../rott/cin_efct.c
  ../../../../rott/cin_evnt.c
  ../../../../rott/cin_glob.c
  ../../../../rott/cin_main.c
  ../../../../rott/cin_util.c
  ../../../../rott/dosutil.c
  ../../../../rott/engine.c
  ../../../../rott/isr.c
  ../../../../rott/modexlib.c
  ../../../../rott/rt_actor.c
  ../../../../rott/rt_battl.c
  ../../../../rott/rt_build.c
  ../../../../rott/rt_cfg.c
  ../../../../rott/rt_com.c
  ../../../../rott/rt_crc.c
  ../../../../rott/rt_debug.c
  ../../../../rott/rt_dmand.c
  ../../../../rott/rt_door.c
  ../../../../rott/rt_draw.c
  ../../../../rott/rt_err.c
  ../../../../rott/rt_floor.c
  ../../../../rott/rt_game.c
  ../../../../rott/rt_in.c
  ../../../../rott/rt_main.c
  ../../../../rott/rt_map.c
  ../../../../rott/rt_menu.c
  ../../../../rott/rt_msg.c
  ../../../../rott/rt_net.c
  ../../../../rott/rt_playr.c
  ../../../../rott/rt_rand.c
  ../../../../rott/rt_scale.c
  ../../../../rott/rt_sound.c
  ../../../../rott/rt_spbal.c
  ../../../../rott/rt_sqrt.c
  ../../../../rott/rt_stat.c
  ../../../../rott/rt_state.c
  ../../../../rott/rt_str.c
  ../../../../rott/rt_swift.c
  ../../../../rott/rt_ted.c
  ../../../../rott/rt_util.c
  ../../../../rott/rt_view.c
  ../../../../rott/rt_vid.c
  ../../../../rott/rt_err.c
  ../../../../rott/scriplib.c
  ../../../../rott/w_wad.c
  ../../../../rott/watcom.c
  ../../../../rott/z_zone.c
  ../../../../rott/byteordr.c
  ../../../../rott/dukemusc.c
  ../../../../rott/winrott.c
  ../../../../rott/audiolib/fx_man.c
  ../../../../rott/audiolib/dsl.c
  ../../../../rott/audiolib/ll_man.c
  ../../../../rott/audiolib/multivoc.c
  ../../../../rott/audiolib/mv_mix.c
  ../../../../rott/audiolib/mvreverb.c
  ../../../../rott/audiolib/nodpmi.c
  ../../../../rott/audiolib/pitch.c
  ../../../../rott/audiolib/user.c
  ../../../../rott/audiolib/usrhooks.c
  src/io_github_h4mu_rott94_ContentPrepareActivity.c
  ${SDL3_SOURCE_DIR}/src/main/android/SDL_android_main.c
)

target_include_directories(main PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../../../rott"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../../../rott/audiolib"
)

target_compile_features(main PRIVATE c_std_99)
target_compile_definitions(main PRIVATE
  USE_SDL=1
  PLATFORM_UNIX=1
  S_IREAD=S_IRUSR
  S_IWRITE=S_IWUSR
  S_IEXEC=S_IXUSR
)

set(_rott_shareware 0)
set(_rott_superrott 0)
set(_rott_sitelicense 0)
if(ROTT_SHAREWARE)
  set(_rott_shareware 1)
elseif(ROTT_SUPERROTT)
  set(_rott_superrott 1)
elseif(ROTT_SITELICENSE)
  set(_rott_sitelicense 1)
endif()

target_compile_definitions(main PRIVATE
  SHAREWARE=${_rott_shareware}
  SUPERROTT=${_rott_superrott}
  SITELICENSE=${_rott_sitelicense}
)

target_link_libraries(main PRIVATE SDL3::SDL3 SDL3_mixer::SDL3_mixer)

if(TARGET hidapi)
  target_link_libraries(main PRIVATE hidapi)
elseif(TARGET hidapi::hidapi)
  target_link_libraries(main PRIVATE hidapi::hidapi)
endif()

find_library(GLESV1_LIB GLESv1_CM)
find_library(GLESV2_LIB GLESv2)
find_library(LOG_LIB log)

target_link_libraries(main PRIVATE
  ${GLESV1_LIB}
  ${GLESV2_LIB}
  ${LOG_LIB}
)
