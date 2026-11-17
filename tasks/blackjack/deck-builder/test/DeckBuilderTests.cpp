#include <DeckBuilder.h>

#include <gmock/gmock.h>

#include <algorithm>
#include <array>

namespace test
{
using namespace ::testing;

namespace
{
class DeckBuilder : public Test
{
protected:
    void TearDown() override
    {
        cardListFree(&deck);
    }

    size_t countOf(Rank rank) const
    {
        return std::count_if(deck.cards, deck.cards + deck.count, [rank](const Card& card) { return card.rank == rank; });
    }

    CardList deck{};
};
} // namespace

TEST_F(DeckBuilder, DeckBuildReturnsFiftyTwoCardsForOneDeck)
{
    // when
    deck = deckBuild(1);

    // then
    EXPECT_THAT(deck.count, Eq(52u));
}

TEST_F(DeckBuilder, DeckBuildReturnsThreeHundredTwelveCardsForSixDecks)
{
    // when
    deck = deckBuild(6);

    // then
    EXPECT_THAT(deck.count, Eq(312u));
}

TEST_F(DeckBuilder, DeckBuildReturnsFourAcesPerDeck)
{
    // when
    deck = deckBuild(1);

    // then
    EXPECT_THAT(countOf(RANK_ACE), Eq(4u));
}

TEST_F(DeckBuilder, DeckBuildReturnsFourKingsPerDeck)
{
    // when
    deck = deckBuild(1);

    // then
    EXPECT_THAT(countOf(RANK_KING), Eq(4u));
}

TEST_F(DeckBuilder, DeckBuildReturnsTwelveTensWhenThreeDecks)
{
    // when
    deck = deckBuild(3);

    // then
    EXPECT_THAT(countOf(RANK_TEN), Eq(12u));
}

TEST_F(DeckBuilder, DeckBuildReturnsEmptyListForZeroDecks)
{
    // when
    deck = deckBuild(0);

    // then
    EXPECT_THAT(deck.count, Eq(0u));
}

TEST_F(DeckBuilder, DeckBuildReturnsNullCardsForZeroDecks)
{
    // when
    deck = deckBuild(0);

    // then
    EXPECT_THAT(deck.cards, IsNull());
}
} // namespace test
