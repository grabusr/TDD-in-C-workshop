set(LIB_NAME CMallocPathfinder)

# Production library - Pathfinder.c calls plain malloc()/calloc()/free() from libc.
# It depends on the map library the same way it would depend on an external static library.
add_library(${LIB_NAME} STATIC Pathfinder.c)
add_library(MallocWrapping::CPathfinder ALIAS ${LIB_NAME})

target_include_directories(${LIB_NAME} INTERFACE .)

target_link_libraries(${LIB_NAME}
        PUBLIC
            MallocWrapping::CMap
)
