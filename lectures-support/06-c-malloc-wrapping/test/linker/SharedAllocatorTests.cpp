#include <AllocatorMock.h>
#include <Pathfinder.h>

#include <gmock/gmock.h>

/*
 * Linker wrapping: there is only one __wrap_malloc for the whole executable. The pathfinder AND
 * the map library it depends on are both routed to the same mock - a fault meant for the
 * pathfinder hits the map as well.
 */
namespace test
{
using namespace ::testing;
using mocks::AllocatorMock;

namespace
{
class GloballyWrappedLibraries : public Test
{
protected:
    void SetUp() override
    {
        map = mapCreate(3, 3);
    }

    void TearDown() override
    {
        pathDestroy(&path);
        mapDestroy(otherMap);
        mapDestroy(map);
    }

    /* Registered as the pathfinder mock, but pathfinderAllocatorMock and mapAllocatorMock are the same slot */
    NiceMock<AllocatorMock> allocatorMock{mocks::pathfinderAllocatorMock};
    Map* map = nullptr;
    Map* otherMap = nullptr;
    Path path{};
};
} // namespace

TEST_F(GloballyWrappedLibraries, FindPathAllocatesSearchStateWithSharedAllocator)
{
    // expected
    EXPECT_CALL(allocatorMock, calloc).Times(AtLeast(1));

    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 2, .y = 2});
}

TEST_F(GloballyWrappedLibraries, FindPathReturnsEmptyPathWhenAllocationFails)
{
    // given
    ON_CALL(allocatorMock, malloc).WillByDefault(ReturnNull());

    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 2, .y = 2});

    // then
    EXPECT_THAT(pathIsEmpty(&path), IsTrue());
}

TEST_F(GloballyWrappedLibraries, CreateMapReturnsNullWhenPathfinderAllocationFails)
{
    // given
    ON_CALL(allocatorMock, malloc).WillByDefault(ReturnNull());

    // when
    otherMap = mapCreate(2, 2);

    // then
    EXPECT_THAT(otherMap, IsNull());
}

} // namespace test
