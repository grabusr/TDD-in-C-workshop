#pragma once

#include <cstddef>
#include <map>
#include <vector>

extern "C"
{
    /// Replacements of malloc() and free() used by the application code (see CMakeLists.txt of this module).
    void* heapFakeMalloc(std::size_t size);
    void heapFakeFree(void* pointer);
}

namespace test
{

/// Replaces the heap of the tested application for the lifetime of the object. Every allocation is recorded, free()
/// only records the release, so an address is never reused and freeing it twice is detected. The memory is really
/// released in the destructor. Only one instance may exist at a time.
class HeapFake
{
public:
    HeapFake();
    ~HeapFake();

    HeapFake(const HeapFake&) = delete;
    HeapFake& operator=(const HeapFake&) = delete;

    void* Malloc(std::size_t size);
    void Free(void* pointer);

    std::size_t AllocationsCount() const;
    /// Blocks allocated and not freed so far.
    std::vector<void*> NotFreedBlocks() const;
    /// Blocks freed more than once.
    std::vector<void*> FreedManyTimesBlocks() const;
    /// Pointers passed to free() that were never returned by malloc().
    const std::vector<void*>& UnknownFreedPointers() const;

private:
    struct Block
    {
        std::size_t size;
        unsigned freeCount;
    };

    std::map<void*, Block> blocks;
    std::vector<void*> unknownFreedPointers;
};

} // namespace test
