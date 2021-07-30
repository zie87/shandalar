cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

project(shandalar LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_library(${PROJECT_NAME}_options INTERFACE)
include(Sanitizers)
enable_sanitizers(${PROJECT_NAME}_options)
add_library(SGE::options ALIAS ${PROJECT_NAME}_options)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
