#include <CardShuffler.h>
#include <RandomGeneratorMock.h>

#include <gmock/gmock.h>

#include <vector>

bool operator==(const Card& lhs, const Card& rhs)
{
    return lhs.rank == rhs.rank;
}

void PrintTo(const Card& card, std::ostream* os)
{
    *os << "Card{rank=" << card.rank << "}";
}

namespace test
{
using namespace ::testing;

namespace
{
class CardShuffler : public Test
{
protected:
    void SetUp() override
    {
        ON_CALL(generator, next).WillByDefault([this](size_t, size_t) {
            return drawnIndexes[drawCount++];
        });
    }

    NiceMock<mocks::RandomGeneratorMock> generator;
    std::vector<size_t> drawnIndexes;
    size_t drawCount{0};
    std::vector<Card> cards;
};
} // namespace

TEST_F(CardShuffler, CardListShuffleDoesNotUseGeneratorWhenListIsEmpty)
{
    // given
    CardList list{nullptr, 0};

    // expected
    EXPECT_CALL(generator, next).Times(0);

    // when
    cardListShuffle(&list, &generator.mock);
}

TEST_F(CardShuffler, CardListShuffleDoesNotUseGeneratorWhenListHasOneCard)
{
    // given
    cards = {{RANK_ACE}};
    CardList list{cards.data(), cards.size()};

    // expected
    EXPECT_CALL(generator, next).Times(0);

    // when
    cardListShuffle(&list, &generator.mock);
}

TEST_F(CardShuffler, CardListShuffleUsesGeneratorOncePerCardWhenListHasMoreThanOneCard)
{
    // given
    cards = {{RANK_ACE}, {RANK_TWO}, {RANK_THREE}};
    CardList list{cards.data(), cards.size()};
    drawnIndexes = {0, 1, 2};

    // expected
    EXPECT_CALL(generator, next).Times(3);

    // when
    cardListShuffle(&list, &generator.mock);
}

TEST_F(CardShuffler, CardListShuffleDrawsIndexesFromWholeListRange)
{
    // given
    cards = {{RANK_ACE}, {RANK_TWO}, {RANK_THREE}};
    CardList list{cards.data(), cards.size()};
    drawnIndexes = {0, 1, 2};

    // expected
    EXPECT_CALL(generator, next(0u, 2u)).Times(AtLeast(1));

    // when
    cardListShuffle(&list, &generator.mock);
}

TEST_F(CardShuffler, CardListShuffleKeepsOrderWhenDrawnIndexesAreCurrentPositions)
{
    // given
    cards = {{RANK_ACE}, {RANK_TWO}, {RANK_THREE}};
    CardList list{cards.data(), cards.size()};
    drawnIndexes = {0, 1, 2};

    // when
    cardListShuffle(&list, &generator.mock);

    // then
    EXPECT_THAT(cards, ElementsAre(Card{RANK_ACE}, Card{RANK_TWO}, Card{RANK_THREE}));
}

TEST_F(CardShuffler, CardListShuffleSwapsCardsWhenDrawnIndexIsDifferent)
{
    // given
    cards = {{RANK_ACE}, {RANK_TWO}};
    CardList list{cards.data(), cards.size()};
    drawnIndexes = {1, 1};

    // when
    cardListShuffle(&list, &generator.mock);

    // then
    EXPECT_THAT(cards, ElementsAre(Card{RANK_TWO}, Card{RANK_ACE}));
}

TEST_F(CardShuffler, CardListShuffleReturnsCardsInOrderResultingFromDrawnIndexes)
{
    // given
    cards = {{RANK_ACE},
             {RANK_TWO},
             {RANK_THREE},
             {RANK_FOUR},
             {RANK_FIVE},
             {RANK_SIX},
             {RANK_SEVEN},
             {RANK_EIGHT},
             {RANK_NINE},
             {RANK_TEN},
             {RANK_JACK},
             {RANK_QUEEN},
             {RANK_KING}};
    CardList list{cards.data(), cards.size()};
    drawnIndexes = {12, 3, 3, 0, 7, 5, 9, 1, 11, 6, 2, 12, 4};

    // when
    cardListShuffle(&list, &generator.mock);

    // then
    EXPECT_THAT(cards,
                ElementsAre(Card{RANK_THREE},
                            Card{RANK_FIVE},
                            Card{RANK_JACK},
                            Card{RANK_KING},
                            Card{RANK_NINE},
                            Card{RANK_SIX},
                            Card{RANK_SEVEN},
                            Card{RANK_FOUR},
                            Card{RANK_QUEEN},
                            Card{RANK_TEN},
                            Card{RANK_TWO},
                            Card{RANK_ACE},
                            Card{RANK_EIGHT}));
}

TEST_F(CardShuffler, CardListShuffleKeepsAllRanksWhenCardsAreShuffled)
{
    // given
    cards = {{RANK_ACE},
             {RANK_TWO},
             {RANK_THREE},
             {RANK_FOUR},
             {RANK_FIVE},
             {RANK_SIX},
             {RANK_SEVEN},
             {RANK_EIGHT},
             {RANK_NINE},
             {RANK_TEN},
             {RANK_JACK},
             {RANK_QUEEN},
             {RANK_KING}};
    CardList list{cards.data(), cards.size()};
    drawnIndexes = {12, 3, 3, 0, 7, 5, 9, 1, 11, 6, 2, 12, 4};

    // when
    cardListShuffle(&list, &generator.mock);

    // then
    EXPECT_THAT(cards,
                UnorderedElementsAre(Card{RANK_ACE},
                                     Card{RANK_TWO},
                                     Card{RANK_THREE},
                                     Card{RANK_FOUR},
                                     Card{RANK_FIVE},
                                     Card{RANK_SIX},
                                     Card{RANK_SEVEN},
                                     Card{RANK_EIGHT},
                                     Card{RANK_NINE},
                                     Card{RANK_TEN},
                                     Card{RANK_JACK},
                                     Card{RANK_QUEEN},
                                     Card{RANK_KING}));
}
} // namespace test
