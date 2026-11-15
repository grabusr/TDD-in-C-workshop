#include <AllocatorMock.h>
#include <Pathfinder.h>

#include <gmock/gmock.h>

/*
 * Preprocessor wrapping: Map.c and Pathfinder.c were compiled with different wrapper headers,
 * so each library talks to its own mock and faults can be injected into one library only.
 */
namespace test
{
using namespace ::testing;
using mocks::AllocatorMock;

namespace
{
class SeparatelyWrappedLibraries : public Test
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

    NiceMock<AllocatorMock> mapMock{mocks::mapAllocatorMock};
    NiceMock<AllocatorMock> pathfinderMock{mocks::pathfinderAllocatorMock};
    Map* map = nullptr;
    Map* otherMap = nullptr;
    Path path{};
};
} // namespace

TEST_F(SeparatelyWrappedLibraries, FindPathAllocatesSearchStateWithPathfinderAllocator)
{
    // expected
    EXPECT_CALL(pathfinderMock, calloc).Times(AtLeast(1));

    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 2, .y = 2});
}

TEST_F(SeparatelyWrappedLibraries, FindPathDoesNotAllocateWithMapAllocator)
{
    // expected
    EXPECT_CALL(mapMock, malloc).Times(0);

    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 2, .y = 2});
}

TEST_F(SeparatelyWrappedLibraries, FindPathReturnsEmptyPathWhenPathfinderAllocationFails)
{
    // given
    ON_CALL(pathfinderMock, malloc).WillByDefault(ReturnNull());

    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 2, .y = 2});

    // then
    EXPECT_THAT(pathIsEmpty(&path), IsTrue());
}

TEST_F(SeparatelyWrappedLibraries, CreateMapSucceedsWhenPathfinderAllocationFails)
{
    // given
    ON_CALL(pathfinderMock, malloc).WillByDefault(ReturnNull());

    // when
    otherMap = mapCreate(2, 2);

    // then
    EXPECT_THAT(otherMap, NotNull());
}

} // namespace test
