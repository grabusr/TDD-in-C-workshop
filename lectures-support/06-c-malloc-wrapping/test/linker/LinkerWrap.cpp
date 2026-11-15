#include <AllocatorMock.h>

/*
 * Linked with -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=free:
 *   - every undefined reference to malloc in every linked object file (Pathfinder.o, Map.o, gtest...)
 *     resolves to __wrap_malloc,
 *   - __real_malloc resolves to the original libc malloc.
 * Calls made from inside shared libraries (libstdc++, libc) are NOT wrapped.
 */
extern "C"
{
void* __real_malloc(size_t size);
void* __real_calloc(size_t count, size_t size);
void __real_free(void* pointer);
}

namespace test::mocks
{
void* realMalloc(const size_t size)
{
    return __real_malloc(size);
}

void* realCalloc(const size_t count, const size_t size)
{
    return __real_calloc(count, size);
}

void realFree(void* const pointer)
{
    __real_free(pointer);
}

namespace
{
AllocatorMock* sharedSlot = nullptr;
} // namespace

/* One __wrap_malloc for the whole executable - both names refer to the same slot */
AllocatorMock*& mapAllocatorMock = sharedSlot;
AllocatorMock*& pathfinderAllocatorMock = sharedSlot;
} // namespace test::mocks

using test::mocks::sharedSlot;

extern "C" void* __wrap_malloc(const size_t size)
{
    return sharedSlot != nullptr ? sharedSlot->malloc(size) : test::mocks::realMalloc(size);
}

extern "C" void* __wrap_calloc(const size_t count, const size_t size)
{
    return sharedSlot != nullptr ? sharedSlot->calloc(count, size) : test::mocks::realCalloc(count, size);
}

extern "C" void __wrap_free(void* const pointer)
{
    sharedSlot != nullptr ? sharedSlot->free(pointer) : test::mocks::realFree(pointer);
}
