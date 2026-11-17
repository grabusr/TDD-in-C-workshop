#include <CardList.h>

#include <gmock/gmock.h>

#include <vector>

namespace test
{
using namespace ::testing;

namespace
{
class CardListText : public Test
{
protected:
    void TearDown() override
    {
        cardListFree(&list);
    }

    CardList list{};
};
} // namespace

TEST_F(CardListText, CreateReturnsListWithCardsForValidText)
{
    // given
    const char* const text = "A K 5\n";

    // when
    list = createCardListFromString(text);

    // then
    EXPECT_THAT(list.cards, NotNull());
}

TEST_F(CardListText, CreateReturnsAllCardsFromText)
{
    // given
    const char* const text = "A K 5\n";

    // when
    list = createCardListFromString(text);

    // then
    EXPECT_THAT(list.count, Eq(3u));
}

TEST_F(CardListText, CreateReturnsCardsInTextOrder)
{
    // given
    const char* const text = "A K 5\n";

    // when
    list = createCardListFromString(text);

    // then
    EXPECT_THAT((std::vector<Card>{list.cards, list.cards + list.count}),
                ElementsAre(Field(&Card::rank, RANK_ACE), Field(&Card::rank, RANK_KING), Field(&Card::rank, RANK_FIVE)));
}

TEST_F(CardListText, CreateReadsCardsFromAllLines)
{
    // given
    const char* const text = "2 J\n10\n";

    // when
    list = createCardListFromString(text);

    // then
    EXPECT_THAT((std::vector<Card>{list.cards, list.cards + list.count}),
                ElementsAre(Field(&Card::rank, RANK_TWO), Field(&Card::rank, RANK_JACK), Field(&Card::rank, RANK_TEN)));
}

TEST_F(CardListText, CreateReadsLastCardWhenTextDoesNotEndWithNewLine)
{
    // given
    const char* const text = "A K 5";

    // when
    list = createCardListFromString(text);

    // then
    EXPECT_THAT((std::vector<Card>{list.cards, list.cards + list.count}),
                ElementsAre(Field(&Card::rank, RANK_ACE), Field(&Card::rank, RANK_KING), Field(&Card::rank, RANK_FIVE)));
}

TEST_F(CardListText, CreateReturnsEmptyListWhenTextContainsUnknownRank)
{
    // given
    const char* const text = "A X 5\n";

    // when
    list = createCardListFromString(text);

    // then
    EXPECT_THAT(list.cards, IsNull());
}

TEST_F(CardListText, CreateReturnsEmptyListWhenTextIsEmpty)
{
    // given
    const char* const text = "";

    // when
    list = createCardListFromString(text);

    // then
    EXPECT_THAT(list.cards, IsNull());
}

TEST_F(CardListText, CreateReturnsEmptyListWhenTextContainsOnlyWhitespace)
{
    // given
    const char* const text = "   \n\t \n";

    // when
    list = createCardListFromString(text);

    // then
    EXPECT_THAT(list.cards, IsNull());
}

TEST_F(CardListText, CreateReturnsEmptyListWhenTextIsNull)
{
    // given
    const char* const text = nullptr;

    // when
    list = createCardListFromString(text);

    // then
    EXPECT_THAT(list.cards, IsNull());
}

TEST_F(CardListText, CreateReturnsZeroCountWhenTextContainsUnknownRank)
{
    // given
    const char* const text = "A X 5\n";

    // when
    list = createCardListFromString(text);

    // then
    EXPECT_THAT(list.count, Eq(0u));
}
} // namespace test
