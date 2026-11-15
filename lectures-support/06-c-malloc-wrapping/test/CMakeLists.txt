find_package(GTest REQUIRED)

set(MAP_DIR ${PROJECT_SOURCE_DIR}/map)
set(PATHFINDER_DIR ${PROJECT_SOURCE_DIR}/pathfinder)

# Mock shared by both wrapping techniques. Every technique defines on its own:
#   - realMalloc()/realCalloc()/realFree() - access to the libc allocator,
#   - mapAllocatorMock/pathfinderAllocatorMock - slots the wrapped calls are routed to.
add_library(CAllocatorMock STATIC mocks/AllocatorMock.cpp)
add_library(MallocWrapping::CAllocatorMock ALIAS CAllocatorMock)
target_include_directories(CAllocatorMock PUBLIC mocks)
target_link_libraries(CAllocatorMock PUBLIC GTest::gmock)

# -------------------------------------------------------------------------------------------------
# Technique 1: preprocessor (compile-time) wrapping - works with every compiler/linker
#
# Every library is compiled once again with its OWN force-included header:
#   Map.c        + MapAllocatorWrapper.h        -> mapMalloc()/mapCalloc()/mapFree()
#   Pathfinder.c + PathfinderAllocatorWrapper.h -> pathfinderMalloc()/pathfinderCalloc()/pathfinderFree()
# so each library can be routed to a different mock. Code that is not recompiled this way
# (e.g. a prebuilt external library) keeps calling libc directly.
# -------------------------------------------------------------------------------------------------
add_library(CMallocMapPreprocessorWrapped STATIC ${MAP_DIR}/Map.c)
target_include_directories(CMallocMapPreprocessorWrapped PUBLIC ${MAP_DIR})
target_compile_options(CMallocMapPreprocessorWrapped
        PRIVATE
            "SHELL:-include ${CMAKE_CURRENT_SOURCE_DIR}/preprocessor/MapAllocatorWrapper.h"
)

add_library(CMallocPathfinderPreprocessorWrapped STATIC ${PATHFINDER_DIR}/Pathfinder.c)
target_include_directories(CMallocPathfinderPreprocessorWrapped PUBLIC ${PATHFINDER_DIR})
target_compile_options(CMallocPathfinderPreprocessorWrapped
        PRIVATE
            "SHELL:-include ${CMAKE_CURRENT_SOURCE_DIR}/preprocessor/PathfinderAllocatorWrapper.h"
)
target_link_libraries(CMallocPathfinderPreprocessorWrapped PUBLIC CMallocMapPreprocessorWrapped)

add_executable(CMallocPreprocessorWrapTests
        MapAllocationTests.cpp
        preprocessor/SeparateAllocatorsTests.cpp
        preprocessor/AllocatorWrappers.cpp
)
target_link_libraries(CMallocPreprocessorWrapTests
        PRIVATE
            GTest::gmock_main
            MallocWrapping::CAllocatorMock
            CMallocPathfinderPreprocessorWrapped
)

# -------------------------------------------------------------------------------------------------
# Technique 2: linker wrapping - GNU ld only (Linux / Docker image), Apple ld64 has no --wrap
#
# Production libraries are linked as they are. "--wrap=malloc" is global for the whole link:
# every undefined reference to malloc in EVERY linked object file - the pathfinder, the map it
# depends on, any other static library (gtest included) - resolves to the single __wrap_malloc.
# Calls made inside shared libraries (libc, libstdc++) are not wrapped.
# -------------------------------------------------------------------------------------------------
if (NOT APPLE)
    add_executable(CMallocLinkerWrapTests
            MapAllocationTests.cpp
            linker/SharedAllocatorTests.cpp
            linker/LinkerWrap.cpp
    )
    target_link_libraries(CMallocLinkerWrapTests
            PRIVATE
                GTest::gmock_main
                MallocWrapping::CAllocatorMock
                MallocWrapping::CPathfinder
    )
    target_link_options(CMallocLinkerWrapTests
            PRIVATE
                LINKER:--wrap=malloc
                LINKER:--wrap=calloc
                LINKER:--wrap=free
    )
endif ()
