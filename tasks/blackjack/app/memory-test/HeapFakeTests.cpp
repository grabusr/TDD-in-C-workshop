#include "HeapFake.h"

#include <gmock/gmock.h>

namespace test
{
using namespace ::testing;

namespace
{
class HeapFakeTest : public Test
{
protected:
    HeapFake heap;
};
} // namespace

TEST_F(HeapFakeTest, NotFreedBlocksReturnsAllocatedBlockWhenNotFreed)
{
    // when
    void* const block = heapFakeMalloc(16);

    // then
    EXPECT_THAT(heap.NotFreedBlocks(), ElementsAre(block));
}

TEST_F(HeapFakeTest, NotFreedBlocksReturnsNothingWhenBlockFreed)
{
    // given
    void* const block = heapFakeMalloc(16);

    // when
    heapFakeFree(block);

    // then
    EXPECT_THAT(heap.NotFreedBlocks(), IsEmpty());
}

TEST_F(HeapFakeTest, FreedManyTimesBlocksReturnsBlockWhenFreedTwice)
{
    // given
    void* const block = heapFakeMalloc(16);

    // when
    heapFakeFree(block);
    heapFakeFree(block);

    // then
    EXPECT_THAT(heap.FreedManyTimesBlocks(), ElementsAre(block));
}

TEST_F(HeapFakeTest, FreedManyTimesBlocksReturnsNothingWhenFreedOnce)
{
    // given
    void* const block = heapFakeMalloc(16);

    // when
    heapFakeFree(block);

    // then
    EXPECT_THAT(heap.FreedManyTimesBlocks(), IsEmpty());
}

TEST_F(HeapFakeTest, MallocDoesNotReuseAddressOfFreedBlock)
{
    // given
    void* const first = heapFakeMalloc(16);
    heapFakeFree(first);

    // when
    void* const second = heapFakeMalloc(16);

    // then
    EXPECT_THAT(second, Ne(first));
}

TEST_F(HeapFakeTest, UnknownFreedPointersReturnsPointerNotAllocatedByHeap)
{
    // given
    int onStack = 0;

    // when
    heapFakeFree(&onStack);

    // then
    EXPECT_THAT(heap.UnknownFreedPointers(), ElementsAre(&onStack));
}

TEST_F(HeapFakeTest, FreeIgnoresNullPointer)
{
    // when
    heapFakeFree(nullptr);

    // then
    EXPECT_THAT(heap.UnknownFreedPointers(), IsEmpty());
}

TEST_F(HeapFakeTest, AllocationsCountReturnsNumberOfMallocCalls)
{
    // when
    heapFakeMalloc(1);
    heapFakeMalloc(2);

    // then
    EXPECT_THAT(heap.AllocationsCount(), Eq(2u));
}
} // namespace test
