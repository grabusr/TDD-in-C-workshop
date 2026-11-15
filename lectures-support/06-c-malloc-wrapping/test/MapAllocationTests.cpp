#include <AllocatorMock.h>
#include <Map.h>

#include <gmock/gmock.h>

#include <cstddef>

namespace test
{
using namespace ::testing;
using mocks::AllocatorMock;

namespace
{
/* Every malloc()/free() made by Map.c goes to mapMock instead of libc - in both wrapping techniques. */
class MallocMockedMap : public Test
{
protected:
    void SetUp() override
    {
        /*
         * Once a test adds EXPECT_CALL for a specific argument, calls with other arguments would become
         * "unexpected". These catch-all expectations allow them; newer EXPECT_CALLs in tests take precedence.
         */
        EXPECT_CALL(mapMock, malloc).Times(AnyNumber());
        EXPECT_CALL(mapMock, free).Times(AnyNumber());
    }

    static constexpr size_t width = 4;
    static constexpr size_t height = 3;
    /* 48 bytes - must differ from sizeof(struct Map) so both allocations are distinguishable */
    static constexpr size_t costsSize = width * height * sizeof(MapCost);

    /* Memory handed out instead of libc - lets tests check exactly what gets released */
    alignas(std::max_align_t) std::byte fakeMemory[costsSize]{};

    NiceMock<AllocatorMock> mapMock{mocks::mapAllocatorMock};
    Map* map = nullptr;
};

class MapCreation : public MallocMockedMap
{
protected:
    void TearDown() override
    {
        mapDestroy(map);
    }
};

class MapDestruction : public MallocMockedMap
{
};
} // namespace

TEST_F(MapCreation, CreateAllocatesCostForEveryField)
{
    // expected
    EXPECT_CALL(mapMock, malloc(costsSize));

    // when
    map = mapCreate(width, height);
}

TEST_F(MapCreation, CreateDoesNotAllocateWhenWidthIsZero)
{
    // expected
    EXPECT_CALL(mapMock, malloc).Times(0);

    // when
    map = mapCreate(0, height);
}

TEST_F(MapCreation, CreateReturnsNullWhenMapAllocationFails)
{
    // given
    ON_CALL(mapMock, malloc(Ne(costsSize))).WillByDefault(ReturnNull());

    // when
    map = mapCreate(width, height);

    // then
    EXPECT_THAT(map, IsNull());
}

TEST_F(MapCreation, CreateDoesNotAllocateCostsWhenMapAllocationFails)
{
    // given
    ON_CALL(mapMock, malloc(Ne(costsSize))).WillByDefault(ReturnNull());

    // expected
    EXPECT_CALL(mapMock, malloc(costsSize)).Times(0);

    // when
    map = mapCreate(width, height);
}

TEST_F(MapCreation, CreateReturnsNullWhenCostsAllocationFails)
{
    // given
    ON_CALL(mapMock, malloc(costsSize)).WillByDefault(ReturnNull());

    // when
    map = mapCreate(width, height);

    // then
    EXPECT_THAT(map, IsNull());
}

TEST_F(MapCreation, CreateReleasesMapWhenCostsAllocationFails)
{
    // given
    ON_CALL(mapMock, malloc(Ne(costsSize))).WillByDefault(Return(fakeMemory));
    ON_CALL(mapMock, malloc(costsSize)).WillByDefault(ReturnNull());

    // expected
    EXPECT_CALL(mapMock, free(fakeMemory)).WillOnce(Return());

    // when
    map = mapCreate(width, height);
}

TEST_F(MapDestruction, DestroyReleasesCosts)
{
    // given
    ON_CALL(mapMock, malloc(costsSize)).WillByDefault(Return(fakeMemory));
    map = mapCreate(width, height);

    // expected
    EXPECT_CALL(mapMock, free(fakeMemory)).WillOnce(Return());

    // when
    mapDestroy(map);
}

TEST_F(MapDestruction, DestroyReleasesMap)
{
    // given
    ON_CALL(mapMock, malloc(Ne(costsSize))).WillByDefault(Return(fakeMemory));
    map = mapCreate(width, height);

    // expected
    EXPECT_CALL(mapMock, free(fakeMemory)).WillOnce(Return());

    // when
    mapDestroy(map);
}

TEST_F(MapDestruction, DestroyDoesNotReleaseAnythingWhenMapIsNull)
{
    // expected
    EXPECT_CALL(mapMock, free).Times(0);

    // when
    mapDestroy(nullptr);
}

} // namespace test
