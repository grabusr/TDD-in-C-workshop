set(LIB_NAME CMallocMap)

# Production library - Map.c calls plain malloc()/free() from libc
add_library(${LIB_NAME} STATIC Map.c)
add_library(MallocWrapping::CMap ALIAS ${LIB_NAME})

target_include_directories(${LIB_NAME} INTERFACE .)
