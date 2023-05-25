set(STRING_ENCODING_TYPE "NONE" CACHE STRING "Set the way strings have to be encoded (ICONV|NONE|...)")
set(KS_STATIC_LIBRARY "ON" CACHE BOOL "Sets whether kaitai-struct should be built as a static runtime")
add_subdirectory(kaitai_struct)
add_subdirectory(raygui/projects/CMake)
