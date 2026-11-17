#include <Hand.h>

#include <gmock/gmock.h>

namespace test
{
using namespace ::testing;

namespace
{
class HandValue : public Test
{
protected:
    Hand hand{};
};
} // namespace

TEST_F(HandValue, HandValueReturnsZeroWhenHandIsEmpty)
{
    // given
    hand.count = 0;

    // when
    const int value = handValue(&hand);

    // then
    EXPECT_THAT(value, Eq(0));
}

TEST_F(HandValue, HandValueReturnsFaceValueForNumberCards)
{
    // given
    hand = {
        {{RANK_TWO}, {RANK_NINE}},
        2
    };

    // when
    const int value = handValue(&hand);

    // then
    EXPECT_THAT(value, Eq(11));
}

TEST_F(HandValue, HandValueCountsTenForTenCard)
{
    // given
    hand = {
        {{RANK_TEN}, {RANK_FIVE}},
        2
    };

    // when
    const int value = handValue(&hand);

    // then
    EXPECT_THAT(value, Eq(15));
}

TEST_F(HandValue, HandValueCountsTenForJack)
{
    // given
    hand = {
        {{RANK_JACK}, {RANK_FIVE}},
        2
    };

    // when
    const int value = handValue(&hand);

    // then
    EXPECT_THAT(value, Eq(15));
}

TEST_F(HandValue, HandValueCountsTenForQueen)
{
    // given
    hand = {
        {{RANK_QUEEN}, {RANK_FIVE}},
        2
    };

    // when
    const int value = handValue(&hand);

    // then
    EXPECT_THAT(value, Eq(15));
}

TEST_F(HandValue, HandValueCountsTenForKing)
{
    // given
    hand = {
        {{RANK_KING}, {RANK_FIVE}},
        2
    };

    // when
    const int value = handValue(&hand);

    // then
    EXPECT_THAT(value, Eq(15));
}

TEST_F(HandValue, HandValueCountsAceAsElevenWhenNotBusting)
{
    // given
    hand = {
        {{RANK_ACE}, {RANK_SEVEN}},
        2
    };

    // when
    const int value = handValue(&hand);

    // then
    EXPECT_THAT(value, Eq(18));
}

TEST_F(HandValue, HandValueCountsAceAsOneWhenElevenWouldBust)
{
    // given
    hand = {
        {{RANK_ACE}, {RANK_SEVEN}, {RANK_NINE}},
        3
    };

    // when
    const int value = handValue(&hand);

    // then
    EXPECT_THAT(value, Eq(17));
}

TEST_F(HandValue, HandValueCountsOnlyOneAceAsElevenForTwoAces)
{
    // given
    hand = {
        {{RANK_ACE}, {RANK_ACE}},
        2
    };

    // when
    const int value = handValue(&hand);

    // then
    EXPECT_THAT(value, Eq(12));
}

TEST_F(HandValue, HandValueReturnsTwentyOneForAceAndKing)
{
    // given
    hand = {
        {{RANK_ACE}, {RANK_KING}},
        2
    };

    // when
    const int value = handValue(&hand);

    // then
    EXPECT_THAT(value, Eq(21));
}

TEST_F(HandValue, HandValueReturnsTwentyOneForTwoAcesAndNine)
{
    // given
    hand = {
        {{RANK_ACE}, {RANK_ACE}, {RANK_NINE}},
        3
    };

    // when
    const int value = handValue(&hand);

    // then
    EXPECT_THAT(value, Eq(21));
}

TEST_F(HandValue, HandValueCountsAceAsOneWhenHandHasTwentyOneWithoutIt)
{
    // given
    hand = {
        {{RANK_KING}, {RANK_QUEEN}, {RANK_ACE}},
        3
    };

    // when
    const int value = handValue(&hand);

    // then
    EXPECT_THAT(value, Eq(21));
}

TEST_F(HandValue, HandValueReturnsValueAboveTwentyOneWhenBust)
{
    // given
    hand = {
        {{RANK_KING}, {RANK_QUEEN}, {RANK_TWO}},
        3
    };

    // when
    const int value = handValue(&hand);

    // then
    EXPECT_THAT(value, Eq(22));
}

TEST_F(HandValue, HandValueIgnoresCardsBeyondCount)
{
    // given
    hand = {
        {{RANK_TWO}, {RANK_THREE}, {RANK_KING}},
        2
    };

    // when
    const int value = handValue(&hand);

    // then
    EXPECT_THAT(value, Eq(5));
}

TEST_F(HandValue, HandValueSumsFullHandOfTwentyTwoCards)
{
    // given
    hand = {
        {{RANK_ACE},   {RANK_ACE},   {RANK_ACE},   {RANK_ACE},   {RANK_TWO},   {RANK_TWO},  {RANK_TWO}, {RANK_TWO},
         {RANK_THREE}, {RANK_THREE}, {RANK_THREE}, {RANK_THREE}, {RANK_ACE},   {RANK_ACE},  {RANK_ACE}, {RANK_ACE},
         {RANK_TWO},   {RANK_TWO},   {RANK_TWO},   {RANK_TWO},   {RANK_THREE}, {RANK_THREE}},
        22
    };

    // when
    const int value = handValue(&hand);

    // then
    EXPECT_THAT(value, Eq(42));
}

namespace
{
class HandOperations : public Test
{
protected:
    Hand hand{};
};
} // namespace

TEST_F(HandOperations, HandAddCardAppendsCardToHand)
{
    // given
    hand = {{{RANK_TWO}}, 1};

    // when
    const bool added = handAddCard(&hand, Card{RANK_NINE});

    // then
    EXPECT_THAT(added, IsTrue());
    EXPECT_THAT(hand.count, Eq(2u));
    EXPECT_THAT(hand.cards[1].rank, Eq(RANK_NINE));
}

TEST_F(HandOperations, HandAddCardRejectsCardWhenHandIsFull)
{
    // given
    hand.count = HAND_MAX_CARDS;

    // when
    const bool added = handAddCard(&hand, Card{RANK_NINE});

    // then
    EXPECT_THAT(added, IsFalse());
    EXPECT_THAT(hand.count, Eq(HAND_MAX_CARDS));
}

TEST_F(HandOperations, HandIsBustReturnsTrueWhenValueAboveTwentyOne)
{
    // given
    hand = {{{RANK_KING}, {RANK_QUEEN}, {RANK_TWO}}, 3};

    // when
    const bool bust = handIsBust(&hand);

    // then
    EXPECT_THAT(bust, IsTrue());
}

TEST_F(HandOperations, HandIsBustReturnsFalseWhenValueIsTwentyOne)
{
    // given
    hand = {{{RANK_KING}, {RANK_QUEEN}, {RANK_ACE}}, 3};

    // when
    const bool bust = handIsBust(&hand);

    // then
    EXPECT_THAT(bust, IsFalse());
}
} // namespace test
