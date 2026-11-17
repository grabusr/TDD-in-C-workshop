#include "HeapFake.h"

#include <cstdlib>

namespace
{
test::HeapFake* activeHeap = nullptr;
} // namespace

extern "C"
{
    void* heapFakeMalloc(const std::size_t size)
    {
        return activeHeap != nullptr ? activeHeap->Malloc(size) : std::malloc(size);
    }

    void heapFakeFree(void* const pointer)
    {
        if (activeHeap != nullptr)
        {
            activeHeap->Free(pointer);
        }
        else
        {
            std::free(pointer);
        }
    }
}

namespace test
{

HeapFake::HeapFake()
{
    activeHeap = this;
}

HeapFake::~HeapFake()
{
    activeHeap = nullptr;
    for (const auto& [pointer, block] : blocks)
    {
        std::free(pointer);
    }
}

void* HeapFake::Malloc(const std::size_t size)
{
    void* const pointer = std::malloc(size);
    if (pointer != nullptr)
    {
        blocks[pointer] = Block{size, 0};
    }
    return pointer;
}

void HeapFake::Free(void* const pointer)
{
    if (pointer == nullptr)
    {
        return;
    }
    const auto block = blocks.find(pointer);
    if (block == blocks.end())
    {
        unknownFreedPointers.push_back(pointer);
        return;
    }
    ++block->second.freeCount;
}

std::size_t HeapFake::AllocationsCount() const
{
    return blocks.size();
}

std::vector<void*> HeapFake::NotFreedBlocks() const
{
    std::vector<void*> result;
    for (const auto& [pointer, block] : blocks)
    {
        if (block.freeCount == 0)
        {
            result.push_back(pointer);
        }
    }
    return result;
}

std::vector<void*> HeapFake::FreedManyTimesBlocks() const
{
    std::vector<void*> result;
    for (const auto& [pointer, block] : blocks)
    {
        if (block.freeCount > 1)
        {
            result.push_back(pointer);
        }
    }
    return result;
}

const std::vector<void*>& HeapFake::UnknownFreedPointers() const
{
    return unknownFreedPointers;
}

} // namespace test
