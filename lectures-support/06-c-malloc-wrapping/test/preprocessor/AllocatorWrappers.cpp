#include <AllocatorMock.h>

#include <cstdlib>

/*
 * Definitions of the functions MapAllocatorWrapper.h and PathfinderAllocatorWrapper.h redirect to.
 * The wrapper headers are deliberately NOT included here - their macros would rewrite
 * malloc/calloc/free in this file and in the gtest headers as well.
 */

namespace test::mocks
{
void* realMalloc(const size_t size)
{
    return std::malloc(size);
}

void* realCalloc(const size_t count, const size_t size)
{
    return std::calloc(count, size);
}

void realFree(void* const pointer)
{
    std::free(pointer);
}

namespace
{
AllocatorMock* mapSlot = nullptr;
AllocatorMock* pathfinderSlot = nullptr;
} // namespace

/* Separate slot for every library */
AllocatorMock*& mapAllocatorMock = mapSlot;
AllocatorMock*& pathfinderAllocatorMock = pathfinderSlot;
} // namespace test::mocks

namespace
{
using test::mocks::AllocatorMock;

void* allocate(AllocatorMock* const mock, const size_t size)
{
    return mock != nullptr ? mock->malloc(size) : test::mocks::realMalloc(size);
}

void* allocateZeroed(AllocatorMock* const mock, const size_t count, const size_t size)
{
    return mock != nullptr ? mock->calloc(count, size) : test::mocks::realCalloc(count, size);
}

void release(AllocatorMock* const mock, void* const pointer)
{
    mock != nullptr ? mock->free(pointer) : test::mocks::realFree(pointer);
}
} // namespace

/* Called by Map.c (MapAllocatorWrapper.h) */
extern "C" void* mapMalloc(const size_t size)
{
    return allocate(test::mocks::mapAllocatorMock, size);
}

extern "C" void* mapCalloc(const size_t count, const size_t size)
{
    return allocateZeroed(test::mocks::mapAllocatorMock, count, size);
}

extern "C" void mapFree(void* const pointer)
{
    release(test::mocks::mapAllocatorMock, pointer);
}

/* Called by Pathfinder.c (PathfinderAllocatorWrapper.h) */
extern "C" void* pathfinderMalloc(const size_t size)
{
    return allocate(test::mocks::pathfinderAllocatorMock, size);
}

extern "C" void* pathfinderCalloc(const size_t count, const size_t size)
{
    return allocateZeroed(test::mocks::pathfinderAllocatorMock, count, size);
}

extern "C" void pathfinderFree(void* const pointer)
{
    release(test::mocks::pathfinderAllocatorMock, pointer);
}
