#include <Map.h>

#include <gmock/gmock.h>

namespace test
{
using namespace ::testing;

namespace
{
class NewMap : public Test
{
protected:
    void SetUp() override
    {
        map = mapCreate(width, height);
    }

    void TearDown() override
    {
        mapDestroy(map);
    }

    static constexpr size_t width = 3;
    static constexpr size_t height = 2;
    Map* map = nullptr;
};
} // namespace

TEST(Map, CreateReturnsNullWhenWidthIsZero)
{
    // when
    Map* map = mapCreate(0, 2);

    // then
    EXPECT_THAT(map, IsNull());
}

TEST(Map, CreateReturnsNullWhenHeightIsZero)
{
    // when
    Map* map = mapCreate(3, 0);

    // then
    EXPECT_THAT(map, IsNull());
}

TEST(Map, DestroyAcceptsNull)
{
    // when
    mapDestroy(nullptr);
}

TEST_F(NewMap, CreateReturnsMap)
{
    // then
    EXPECT_THAT(map, NotNull());
}

TEST_F(NewMap, GetWidthReturnsCreatedWidth)
{
    // when
    const size_t result = mapGetWidth(map);

    // then
    EXPECT_THAT(result, Eq(width));
}

TEST_F(NewMap, GetHeightReturnsCreatedHeight)
{
    // when
    const size_t result = mapGetHeight(map);

    // then
    EXPECT_THAT(result, Eq(height));
}

TEST_F(NewMap, GetCostReturnsDefaultCostWhenNotSet)
{
    // when
    const MapCost result = mapGetCost(map, {.x = 2, .y = 1});

    // then
    EXPECT_THAT(result, Eq(static_cast<MapCost>(mapDefaultCost)));
}

TEST_F(NewMap, SetCostStoresCost)
{
    // given
    mapSetCost(map, {.x = 1, .y = 1}, 7);

    // when
    const MapCost result = mapGetCost(map, {.x = 1, .y = 1});

    // then
    EXPECT_THAT(result, Eq(7U));
}

TEST_F(NewMap, SetCostReturnsTrueWhenCoordinateInsideMap)
{
    // when
    const bool result = mapSetCost(map, {.x = 2, .y = 1}, 7);

    // then
    EXPECT_THAT(result, IsTrue());
}

TEST_F(NewMap, SetCostDoesNotChangeNeighbourField)
{
    // given
    mapSetCost(map, {.x = 1, .y = 1}, 7);

    // when
    const MapCost result = mapGetCost(map, {.x = 2, .y = 1});

    // then
    EXPECT_THAT(result, Eq(static_cast<MapCost>(mapDefaultCost)));
}

TEST_F(NewMap, SetCostDoesNotChangeFieldInNextRow)
{
    // given
    mapSetCost(map, {.x = 0, .y = 0}, 7);

    // when
    const MapCost result = mapGetCost(map, {.x = 0, .y = 1});

    // then
    EXPECT_THAT(result, Eq(static_cast<MapCost>(mapDefaultCost)));
}

TEST_F(NewMap, SetCostRejectsCoordinateOutsideMap)
{
    // when
    const bool result = mapSetCost(map, {.x = 3, .y = 0}, 7);

    // then
    EXPECT_THAT(result, IsFalse());
}

TEST_F(NewMap, SetCostRejectsNegativeCoordinate)
{
    // when
    const bool result = mapSetCost(map, {.x = -1, .y = 0}, 7);

    // then
    EXPECT_THAT(result, IsFalse());
}

TEST_F(NewMap, GetCostReturnsImpassableCostWhenOutsideMap)
{
    // when
    const MapCost result = mapGetCost(map, {.x = 0, .y = 2});

    // then
    EXPECT_THAT(result, Eq(static_cast<MapCost>(mapImpassableCost)));
}

TEST_F(NewMap, ContainsReturnsTrueForLastField)
{
    // when
    const bool result = mapContains(map, {.x = 2, .y = 1});

    // then
    EXPECT_THAT(result, IsTrue());
}

TEST_F(NewMap, ContainsReturnsFalseWhenXEqualsWidth)
{
    // when
    const bool result = mapContains(map, {.x = 3, .y = 1});

    // then
    EXPECT_THAT(result, IsFalse());
}

TEST_F(NewMap, ContainsReturnsFalseWhenYEqualsHeight)
{
    // when
    const bool result = mapContains(map, {.x = 2, .y = 2});

    // then
    EXPECT_THAT(result, IsFalse());
}

TEST_F(NewMap, ContainsReturnsFalseWhenYIsNegative)
{
    // when
    const bool result = mapContains(map, {.x = 0, .y = -1});

    // then
    EXPECT_THAT(result, IsFalse());
}

TEST_F(NewMap, IsWalkableReturnsTrueWhenCostIsPositive)
{
    // given
    mapSetCost(map, {.x = 1, .y = 0}, 5);

    // when
    const bool result = mapIsWalkable(map, {.x = 1, .y = 0});

    // then
    EXPECT_THAT(result, IsTrue());
}

TEST_F(NewMap, IsWalkableReturnsFalseWhenCostIsZero)
{
    // given
    mapSetCost(map, {.x = 1, .y = 0}, mapImpassableCost);

    // when
    const bool result = mapIsWalkable(map, {.x = 1, .y = 0});

    // then
    EXPECT_THAT(result, IsFalse());
}

TEST_F(NewMap, IsWalkableReturnsFalseWhenOutsideMap)
{
    // when
    const bool result = mapIsWalkable(map, {.x = -1, .y = 0});

    // then
    EXPECT_THAT(result, IsFalse());
}

} // namespace test
