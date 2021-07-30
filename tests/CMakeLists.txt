add_subdirectory(doctest EXCLUDE_FROM_ALL)

add_executable(shandalar_tests)
target_sources(shandalar_tests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/palettes_tests.cpp)

set(TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)

target_compile_options(shandalar_tests PRIVATE "-DTEST_DATA_DIR=\"${TEST_DATA_DIR}\"")
target_link_libraries(shandalar_tests PUBLIC SGE::SGE doctest::doctest)

add_test(unittests shandalar_tests)
