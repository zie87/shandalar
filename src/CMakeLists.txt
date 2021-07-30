find_package(SDL2 REQUIRED)

if(NOT TARGET SDL2::SDL2)
    add_library(sdl_wrapper INTERFACE)
    target_include_directories(sdl_wrapper INTERFACE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(sdl_wrapper INTERFACE ${SDL2_LIBRARIES})
    add_library(SDL2::SDL2 ALIAS sdl_wrapper)
endif()

add_library(sge STATIC)

target_sources(sge PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/video/image_pic.cpp
)

target_include_directories(sge PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(sge PUBLIC SDL2::SDL2 SGE::options)
target_compile_features(sge PUBLIC cxx_std_17)

add_library(SGE::SGE ALIAS sge)
