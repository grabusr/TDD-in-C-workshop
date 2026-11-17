#include <SeededRandom.h>

#include <gmock/gmock.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>

namespace test
{
using namespace ::testing;

namespace
{
constexpr size_t kFullRangeMin = 0;
constexpr size_t kFullRangeMax = std::numeric_limits<size_t>::max();

class SeededRandomFixture : public Test
{
protected:
    void SetUp() override
    {
        seededRandomInit(&first, 42);
        seededRandomInit(&second, 42);
        seededRandomInit(&other, 43);
        seededRandomInit(&zeroSeeded, 0);
    }

    size_t draw(SeededRandom& random, size_t min, size_t max)
    {
        return random.generator.next(random.generator.context, min, max);
    }

    std::array<size_t, 5> drawFive(SeededRandom& random, size_t min, size_t max)
    {
        return {draw(random, min, max), draw(random, min, max), draw(random, min, max), draw(random, min, max),
                draw(random, min, max)};
    }

    SeededRandom first;
    SeededRandom second;
    SeededRandom other;
    SeededRandom zeroSeeded;
};

MATCHER_P2(IsBetween, min, max, "")
{
    return arg >= min && arg <= max;
}
} // namespace

TEST_F(SeededRandomFixture, NextReturnsSameSequenceWhenSeedIsSame)
{
    // when
    const auto sequenceOfFirst = drawFive(first, 0, 1000);
    const auto sequenceOfSecond = drawFive(second, 0, 1000);

    // then
    EXPECT_THAT(sequenceOfFirst, Eq(sequenceOfSecond));
}

TEST_F(SeededRandomFixture, NextReturnsDifferentSequenceWhenSeedDiffers)
{
    // when
    const auto sequenceOfFirst = drawFive(first, 0, 1000000);
    const auto sequenceOfOther = drawFive(other, 0, 1000000);

    // then
    EXPECT_THAT(sequenceOfFirst, Ne(sequenceOfOther));
}

TEST_F(SeededRandomFixture, NextReturnsKnownSplitMix64SequenceForSeedZero)
{
    // when
    const auto sequence = drawFive(zeroSeeded, kFullRangeMin, kFullRangeMax);

    // then
    EXPECT_THAT(sequence, ElementsAre(0xE220A8397B1DCDAFull, 0x6E789E6AA1B965F4ull, 0x06C45D188009454Full,
                                      0xF88BB8A8724C81ECull, 0x1B39896A51A8749Bull));
}

TEST_F(SeededRandomFixture, NextReturnsValuesWithinRange)
{
    // when
    const auto sequence = drawFive(first, 3, 9);

    // then
    EXPECT_THAT(sequence, Each(IsBetween(3u, 9u)));
}

TEST_F(SeededRandomFixture, NextReturnsMinWhenRangeHasSingleValue)
{
    // when
    const auto value = draw(first, 7, 7);

    // then
    EXPECT_THAT(value, Eq(7u));
}

TEST_F(SeededRandomFixture, NextReachesBothEndsOfRangeOverManyDraws)
{
    // given
    std::array<size_t, 64> values{};

    // when
    std::generate(values.begin(), values.end(), [this] { return draw(first, 0, 1); });

    // then
    EXPECT_THAT(values, AllOf(Contains(0u), Contains(1u)));
}

} // namespace test
