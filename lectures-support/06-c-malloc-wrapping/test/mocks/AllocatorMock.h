#pragma once

#include <gmock/gmock.h>

#include <cstddef>

namespace test::mocks
{

/* Real libc allocator, defined separately by every wrapping technique. */
void* realMalloc(size_t size);
void* realCalloc(size_t count, size_t size);
void realFree(void* pointer);

/*
 * While an instance exists, every wrapped allocation routed to its slot goes to it.
 * By default it delegates to the real allocator, so tests override only what they need.
 */
class AllocatorMock
{
public:
    explicit AllocatorMock(AllocatorMock*& slot);
    ~AllocatorMock();

    AllocatorMock(const AllocatorMock&) = delete;
    AllocatorMock& operator=(const AllocatorMock&) = delete;

    MOCK_METHOD(void*, malloc, (size_t size));
    MOCK_METHOD(void*, calloc, (size_t count, size_t size));
    MOCK_METHOD(void, free, (void* pointer));

private:
    AllocatorMock*& slot;
};

/*
 * Slots the wrapped allocations are routed to:
 *   - preprocessor wrapping: two independent slots, one per library,
 *   - linker wrapping: both names refer to the SAME slot - __wrap_malloc cannot tell the caller.
 */
extern AllocatorMock*& mapAllocatorMock;
extern AllocatorMock*& pathfinderAllocatorMock;

} // namespace test::mocks
