cmake_minimum_required(VERSION 2.6.4)

include_directories(${CMAKE_CURRENT_LIST_DIR}/gtinclude)

link_directories(${CMAKE_CURRENT_LIST_DIR}/gtlib)

add_subdirectory(recorder)

add_executable(recorder_main main.cc test/recorder/recorder_test.cc)

target_link_libraries(recorder_main recorder -lgtest -lpthread)

add_subdirectory(compressor)
add_subdirectory(decompressor)

add_executable(compressor_main main.cc test/compressor/compressor_test.cc)

target_link_libraries(compressor_main compressor decompressor -lgtest -lpthread)
