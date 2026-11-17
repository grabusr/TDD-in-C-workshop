#include <CardName.h>

#include <gmock/gmock.h>

namespace test
{
using namespace ::testing;

TEST(CardName, CardRankNameReturnsAForAce)
{
    // when
    const std::string name = cardRankName(RANK_ACE);

    // then
    EXPECT_THAT(name, StrEq("A"));
}

TEST(CardName, CardRankNameReturnsDigitForNumberRank)
{
    // when
    const std::string name = cardRankName(RANK_SEVEN);

    // then
    EXPECT_THAT(name, StrEq("7"));
}

TEST(CardName, CardRankNameReturnsTenForTen)
{
    // when
    const std::string name = cardRankName(RANK_TEN);

    // then
    EXPECT_THAT(name, StrEq("10"));
}

TEST(CardName, CardRankNameReturnsJForJack)
{
    // when
    const std::string name = cardRankName(RANK_JACK);

    // then
    EXPECT_THAT(name, StrEq("J"));
}

TEST(CardName, CardRankNameReturnsQForQueen)
{
    // when
    const std::string name = cardRankName(RANK_QUEEN);

    // then
    EXPECT_THAT(name, StrEq("Q"));
}

TEST(CardName, CardRankNameReturnsKForKing)
{
    // when
    const std::string name = cardRankName(RANK_KING);

    // then
    EXPECT_THAT(name, StrEq("K"));
}

TEST(CardName, CardRankNameReturnsQuestionMarkForUnknownRank)
{
    // when
    const std::string name = cardRankName(static_cast<Rank>(99));

    // then
    EXPECT_THAT(name, StrEq("?"));
}
} // namespace test
