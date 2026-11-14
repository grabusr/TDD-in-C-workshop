#include <Pathfinder.h>

#include <gmock/gmock.h>

#include <cstdlib>
#include <ostream>
#include <vector>

bool operator==(const Coordinate& lhs, const Coordinate& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

void PrintTo(const Coordinate& coordinate, std::ostream* os)
{
    *os << "(" << coordinate.x << ", " << coordinate.y << ")";
}

namespace test
{
using namespace ::testing;

namespace
{
MATCHER_P(StartsAt, coordinate, "starts at " + PrintToString(coordinate))
{
    return !arg.empty() && arg.front() == coordinate;
}

MATCHER_P(EndsAt, coordinate, "ends at " + PrintToString(coordinate))
{
    return !arg.empty() && arg.back() == coordinate;
}

MATCHER(IsContinuousPath, "is a non-empty path of 4-directionally adjacent steps")
{
    if (arg.empty())
    {
        return false;
    }
    for (size_t i = 1; i < arg.size(); ++i)
    {
        const int distance = std::abs(arg[i].x - arg[i - 1].x) + std::abs(arg[i].y - arg[i - 1].y);
        if (distance != 1)
        {
            *result_listener << "step " << i << " is not adjacent to the previous one";
            return false;
        }
    }
    return true;
}

MATCHER_P(AllWalkableOn, map, "contains only walkable fields")
{
    for (const Coordinate& coordinate : arg)
    {
        if (!mapIsWalkable(map, coordinate))
        {
            *result_listener << "field " << PrintToString(coordinate) << " is not walkable";
            return false;
        }
    }
    return true;
}

class PathfinderTest : public Test
{
protected:
    void TearDown() override
    {
        pathDestroy(&path);
        mapDestroy(map);
    }

    [[nodiscard]] std::vector<Coordinate> pathPoints() const
    {
        return {path.points, path.points + path.length};
    }

    Map* map = nullptr;
    Path path{};
};

/* 5x5, every field costs 1 */
class OpenMap : public PathfinderTest
{
protected:
    void SetUp() override
    {
        map = mapCreate(5, 5);
    }
};

/* 4x1, single row */
class CorridorMap : public PathfinderTest
{
protected:
    void SetUp() override
    {
        map = mapCreate(4, 1);
    }
};

/*
 * 5x5, wall at x == 2 with a gap at y == 4
 *   . . # . .
 *   . . # . .
 *   . . # . .
 *   . . # . .
 *   . . . . .
 */
class WalledMap : public PathfinderTest
{
protected:
    void SetUp() override
    {
        map = mapCreate(5, 5);
        mapSetCost(map, {.x = 2, .y = 0}, mapImpassableCost);
        mapSetCost(map, {.x = 2, .y = 1}, mapImpassableCost);
        mapSetCost(map, {.x = 2, .y = 2}, mapImpassableCost);
        mapSetCost(map, {.x = 2, .y = 3}, mapImpassableCost);
    }
};

/* 5x5, full wall at x == 2 */
class BlockedMap : public PathfinderTest
{
protected:
    void SetUp() override
    {
        map = mapCreate(5, 5);
        mapSetCost(map, {.x = 2, .y = 0}, mapImpassableCost);
        mapSetCost(map, {.x = 2, .y = 1}, mapImpassableCost);
        mapSetCost(map, {.x = 2, .y = 2}, mapImpassableCost);
        mapSetCost(map, {.x = 2, .y = 3}, mapImpassableCost);
        mapSetCost(map, {.x = 2, .y = 4}, mapImpassableCost);
    }
};

/*
 * 3x3, middle field costs 10
 *   1  1 1
 *   1 10 1
 *   1  1 1
 */
class ExpensiveMiddleMap : public PathfinderTest
{
protected:
    void SetUp() override
    {
        map = mapCreate(3, 3);
        mapSetCost(map, {.x = 1, .y = 1}, 10);
    }
};

/*
 * 3x2, direct route is short but expensive, detour through lower row is cheaper
 *   1 9 1
 *   1 1 1
 */
class CheapDetourMap : public PathfinderTest
{
protected:
    void SetUp() override
    {
        map = mapCreate(3, 2);
        mapSetCost(map, {.x = 1, .y = 0}, 9);
    }
};
} // namespace

TEST(Pathfinder, FindPathReturnsEmptyPathWhenMapIsNull)
{
    // when
    Path path = pathfinderFindPath(nullptr, {.x = 0, .y = 0}, {.x = 1, .y = 0});

    // then
    EXPECT_THAT(path.length, Eq(0U));
}

TEST_F(OpenMap, FindPathReturnsEmptyPathWhenStartOutsideMap)
{
    // when
    path = pathfinderFindPath(map, {.x = -1, .y = 0}, {.x = 4, .y = 4});

    // then
    EXPECT_THAT(pathPoints(), IsEmpty());
}

TEST_F(OpenMap, FindPathReturnsEmptyPathWhenGoalOutsideMap)
{
    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 5, .y = 0});

    // then
    EXPECT_THAT(pathPoints(), IsEmpty());
}

TEST_F(OpenMap, FindPathReturnsEmptyPathWhenGoalIsImpassable)
{
    // given
    mapSetCost(map, {.x = 4, .y = 4}, mapImpassableCost);

    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 4, .y = 4});

    // then
    EXPECT_THAT(pathPoints(), IsEmpty());
}

TEST_F(OpenMap, FindPathReturnsEmptyPathWhenStartIsImpassable)
{
    // given
    mapSetCost(map, {.x = 0, .y = 0}, mapImpassableCost);

    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 4, .y = 4});

    // then
    EXPECT_THAT(pathPoints(), IsEmpty());
}

TEST_F(OpenMap, FindPathReturnsSingleCoordinateWhenStartEqualsGoal)
{
    // when
    path = pathfinderFindPath(map, {.x = 2, .y = 2}, {.x = 2, .y = 2});

    // then
    EXPECT_THAT(pathPoints(), ElementsAre(Coordinate{.x = 2, .y = 2}));
}

TEST_F(OpenMap, FindPathReturnsPathStartingAtStart)
{
    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 4, .y = 4});

    // then
    EXPECT_THAT(pathPoints(), StartsAt(Coordinate{.x = 0, .y = 0}));
}

TEST_F(OpenMap, FindPathReturnsPathEndingAtGoal)
{
    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 4, .y = 4});

    // then
    EXPECT_THAT(pathPoints(), EndsAt(Coordinate{.x = 4, .y = 4}));
}

TEST_F(OpenMap, FindPathReturnsShortestLength)
{
    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 4, .y = 4});

    // then
    EXPECT_THAT(pathPoints(), SizeIs(9));
}

TEST_F(OpenMap, FindPathReturnsAdjacentSteps)
{
    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 4, .y = 4});

    // then
    EXPECT_THAT(pathPoints(), IsContinuousPath());
}

TEST_F(OpenMap, FindPathReturnsNonEmptyPathWhenGoalReachable)
{
    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 4, .y = 4});

    // then
    EXPECT_THAT(pathIsEmpty(&path), IsFalse());
}

TEST_F(CorridorMap, FindPathReturnsStraightLine)
{
    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 3, .y = 0});

    // then
    EXPECT_THAT(pathPoints(),
                ElementsAre(Coordinate{.x = 0, .y = 0},
                            Coordinate{.x = 1, .y = 0},
                            Coordinate{.x = 2, .y = 0},
                            Coordinate{.x = 3, .y = 0}));
}

TEST_F(CorridorMap, FindPathReturnsReversedLineWhenGoalIsBehindStart)
{
    // when
    path = pathfinderFindPath(map, {.x = 3, .y = 0}, {.x = 1, .y = 0});

    // then
    EXPECT_THAT(pathPoints(),
                ElementsAre(Coordinate{.x = 3, .y = 0}, Coordinate{.x = 2, .y = 0}, Coordinate{.x = 1, .y = 0}));
}

TEST_F(WalledMap, FindPathPassesThroughGap)
{
    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 4, .y = 0});

    // then
    EXPECT_THAT(pathPoints(), Contains(Coordinate{.x = 2, .y = 4}));
}

TEST_F(WalledMap, FindPathAvoidsImpassableFields)
{
    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 4, .y = 0});

    // then
    EXPECT_THAT(pathPoints(), AllWalkableOn(map));
}

TEST_F(WalledMap, FindPathReturnsShortestLengthAroundWall)
{
    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 4, .y = 0});

    // then
    EXPECT_THAT(pathPoints(), SizeIs(13));
}

TEST_F(BlockedMap, FindPathReturnsEmptyPathWhenGoalUnreachable)
{
    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 4, .y = 0});

    // then
    EXPECT_THAT(pathPoints(), IsEmpty());
}

TEST_F(ExpensiveMiddleMap, FindPathAvoidsExpensiveField)
{
    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 1}, {.x = 2, .y = 1});

    // then
    EXPECT_THAT(pathPoints(), Not(Contains(Coordinate{.x = 1, .y = 1})));
}

TEST_F(CheapDetourMap, FindPathPrefersCheaperLongerRoute)
{
    // when
    path = pathfinderFindPath(map, {.x = 0, .y = 0}, {.x = 2, .y = 0});

    // then
    EXPECT_THAT(pathPoints(),
                ElementsAre(Coordinate{.x = 0, .y = 0},
                            Coordinate{.x = 0, .y = 1},
                            Coordinate{.x = 1, .y = 1},
                            Coordinate{.x = 2, .y = 1},
                            Coordinate{.x = 2, .y = 0}));
}

TEST(Path, DestroyResetsLength)
{
    // given
    Path path{.points = static_cast<Coordinate*>(std::malloc(sizeof(Coordinate))), .length = 1};

    // when
    pathDestroy(&path);

    // then
    EXPECT_THAT(path.length, Eq(0U));
}

TEST(Path, DestroyResetsPoints)
{
    // given
    Path path{.points = static_cast<Coordinate*>(std::malloc(sizeof(Coordinate))), .length = 1};

    // when
    pathDestroy(&path);

    // then
    EXPECT_THAT(path.points, IsNull());
}

TEST(Path, DestroyAcceptsNull)
{
    // when
    pathDestroy(nullptr);
}

TEST(Path, IsEmptyReturnsTrueWhenLengthIsZero)
{
    // given
    constexpr Path path{.points = nullptr, .length = 0};

    // when
    const bool result = pathIsEmpty(&path);

    // then
    EXPECT_THAT(result, IsTrue());
}

TEST(Path, IsEmptyReturnsFalseWhenLengthIsPositive)
{
    // given
    Coordinate point{.x = 0, .y = 0};
    const Path path{.points = &point, .length = 1};

    // when
    const bool result = pathIsEmpty(&path);

    // then
    EXPECT_THAT(result, IsFalse());
}

} // namespace test
