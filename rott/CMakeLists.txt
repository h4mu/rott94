cmake_minimum_required(VERSION 3.16)

project(rott LANGUAGES C)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

include(GNUInstallDirs)

option(ROTT_SHAREWARE "Build the shareware variant" ON)
option(ROTT_SUPERROTT "Build the Super ROTT variant" OFF)
option(ROTT_SITELICENSE "Build the site license variant" OFF)
set(ROTT_SUFFIX "" CACHE STRING "Executable name suffix, for example -huntbgin")
set(ROTT_DATADIR "" CACHE PATH "Optional data directory embedded in the binary")

if(ROTT_SHAREWARE AND (ROTT_SUPERROTT OR ROTT_SITELICENSE))
  message(FATAL_ERROR "ROTT_SHAREWARE cannot be combined with ROTT_SUPERROTT or ROTT_SITELICENSE")
endif()
if(ROTT_SUPERROTT AND ROTT_SITELICENSE)
  message(FATAL_ERROR "ROTT_SUPERROTT and ROTT_SITELICENSE are mutually exclusive")
endif()

set(SDL3_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../android/app/jni/SDL")
set(SDL3_MIXER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../android/app/jni/SDL_mixer")

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

add_subdirectory("${SDL3_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/_deps/SDL3" EXCLUDE_FROM_ALL)
add_subdirectory("${SDL3_MIXER_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/_deps/SDL3_mixer" EXCLUDE_FROM_ALL)

add_subdirectory(audiolib)

add_executable(rott)
target_sources(rott PRIVATE
  byteordr.c
  cin_actr.c
  cin_efct.c
  cin_evnt.c
  cin_glob.c
  cin_main.c
  cin_util.c
  dosutil.c
  engine.c
  fx_man.c
  isr.c
  modexlib.c
  rt_actor.c
  rt_battl.c
  rt_build.c
  rt_cfg.c
  rt_com.c
  rt_crc.c
  rt_debug.c
  rt_dmand.c
  rt_door.c
  rt_draw.c
  rt_err.c
  rt_floor.c
  rt_game.c
  rt_in.c
  rt_main.c
  rt_map.c
  rt_menu.c
  rt_msg.c
  rt_net.c
  rt_playr.c
  rt_rand.c
  rt_scale.c
  rt_sound.c
  rt_spbal.c
  rt_sqrt.c
  rt_stat.c
  rt_state.c
  rt_str.c
  rt_swift.c
  rt_ted.c
  rt_util.c
  rt_vid.c
  rt_view.c
  scriplib.c
  watcom.c
  winrott.c
  w_wad.c
  z_zone.c
)

target_compile_features(rott PRIVATE c_std_99)
if(NOT MSVC)
  target_compile_options(rott PRIVATE -Wall)
endif()

target_include_directories(rott PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/audiolib"
)

target_compile_definitions(rott PRIVATE
  USE_SDL=1
)

if(WIN32)
  target_compile_definitions(rott PRIVATE PLATFORM_WIN32=1)
else()
  target_compile_definitions(rott PRIVATE PLATFORM_UNIX=1)
endif()

target_compile_definitions(rott PRIVATE
  $<$<CONFIG:Release>:NDEBUG>
  $<$<CONFIG:MinSizeRel>:NDEBUG>
  $<$<CONFIG:RelWithDebInfo>:NDEBUG>
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

target_compile_definitions(rott PRIVATE
  SHAREWARE=${_rott_shareware}
  SUPERROTT=${_rott_superrott}
  SITELICENSE=${_rott_sitelicense}
)

if(ROTT_DATADIR)
  file(TO_CMAKE_PATH "${ROTT_DATADIR}" _rott_datadir)
  string(REGEX REPLACE "/+$" "" _rott_datadir "${_rott_datadir}")
  target_compile_definitions(rott PRIVATE DATADIR=\"${_rott_datadir}/\")
endif()

target_link_libraries(rott PRIVATE
  audiolib
  SDL3::SDL3
  SDL3_mixer::SDL3_mixer
)

if(NOT MSVC)
  target_link_libraries(rott PRIVATE m)
endif()

set_target_properties(rott PROPERTIES
  OUTPUT_NAME "rott${ROTT_SUFFIX}"
)

if(UNIX AND NOT APPLE)
  set_target_properties(rott PROPERTIES
    INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
    INSTALL_RPATH_USE_LINK_PATH TRUE
  )
endif()

install(TARGETS rott
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(UNIX AND NOT APPLE)
  install(CODE "
    set(_rott_exe \"\${CMAKE_INSTALL_PREFIX}/\${CMAKE_INSTALL_BINDIR}/rott${ROTT_SUFFIX}\")
    file(GET_RUNTIME_DEPENDENCIES
      EXECUTABLES \"\${_rott_exe}\"
      RESOLVED_DEPENDENCIES_VAR _rott_deps
      UNRESOLVED_DEPENDENCIES_VAR _rott_unresolved
    )
    foreach(_dep IN LISTS _rott_deps)
      if(_dep MATCHES \"libSDL3.*\" OR _dep MATCHES \"libSDL3_mixer.*\")
        file(INSTALL
          DESTINATION \"\${CMAKE_INSTALL_PREFIX}/\${CMAKE_INSTALL_LIBDIR}\"
          TYPE SHARED_LIBRARY
          FILES \"\${_dep}\"
        )
      endif()
    endforeach()
  ")
endif()
