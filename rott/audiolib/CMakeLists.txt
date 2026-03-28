add_library(audiolib STATIC)
target_sources(audiolib PRIVATE
  dsl.c
  ll_man.c
  multivoc.c
  mv_mix.c
  mvreverb.c
  nodpmi.c
  pitch.c
  user.c
  usrhooks.c
)

target_compile_features(audiolib PRIVATE c_std_99)
if(NOT MSVC)
  target_compile_options(audiolib PRIVATE -Wall)
endif()

target_include_directories(audiolib PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_compile_definitions(audiolib PRIVATE
  PLATFORM_UNIX=1
)

target_link_libraries(audiolib PRIVATE
  SDL3::SDL3
  SDL3_mixer::SDL3_mixer
)
