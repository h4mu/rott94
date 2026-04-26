cmake_minimum_required(VERSION 3.16)

project(rott94_android_compat LANGUAGES C)

get_filename_component(ROTT94_REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../.." ABSOLUTE)

add_subdirectory("${ROTT94_REPO_ROOT}" "${CMAKE_CURRENT_BINARY_DIR}/rott94-root")
