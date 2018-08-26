#aux_source_directory(./ THIS_CMAKE_SOURCE_FILE)
set(THIS_CMAKE_SOURCE_FILE ./visualization.c)
add_library(decompressor STATIC ${THIS_CMAKE_SOURCE_FILE})
