#include <CardList.h>

#include <gmock/gmock.h>

#include <vector>

namespace test
{
using namespace ::testing;

namespace
{
class CardListFile : public Test
{
protected:
    void TearDown() override
    {
        cardListFree(&list);
    }

    CardList list{};
};
} // namespace

TEST_F(CardListFile, LoadReturnsTrueForValidFile)
{
    // when
    const bool loaded = cardListLoad(TEST_DATA_DIR "/three_cards.txt", &list);

    // then
    EXPECT_THAT(loaded, IsTrue());
}

TEST_F(CardListFile, LoadReturnsAllCardsFromFile)
{
    // when
    cardListLoad(TEST_DATA_DIR "/three_cards.txt", &list);

    // then
    EXPECT_THAT(list.count, Eq(3u));
}

TEST_F(CardListFile, LoadReturnsCardsInFileOrder)
{
    // when
    cardListLoad(TEST_DATA_DIR "/three_cards.txt", &list);

    // then
    EXPECT_THAT((std::vector<Card>{list.cards, list.cards + list.count}),
                ElementsAre(Field(&Card::rank, RANK_ACE), Field(&Card::rank, RANK_KING), Field(&Card::rank, RANK_FIVE)));
}

TEST_F(CardListFile, LoadReadsCardsFromAllLines)
{
    // when
    cardListLoad(TEST_DATA_DIR "/multiline.txt", &list);

    // then
    EXPECT_THAT((std::vector<Card>{list.cards, list.cards + list.count}),
                ElementsAre(Field(&Card::rank, RANK_TWO), Field(&Card::rank, RANK_JACK), Field(&Card::rank, RANK_TEN)));
}

TEST_F(CardListFile, LoadReturnsFalseWhenFileContainsUnknownRank)
{
    // when
    const bool loaded = cardListLoad(TEST_DATA_DIR "/invalid_card.txt", &list);

    // then
    EXPECT_THAT(loaded, IsFalse());
}

TEST_F(CardListFile, LoadReturnsFalseWhenFileIsEmpty)
{
    // when
    const bool loaded = cardListLoad(TEST_DATA_DIR "/empty.txt", &list);

    // then
    EXPECT_THAT(loaded, IsFalse());
}

TEST_F(CardListFile, LoadReturnsFalseWhenFileContainsOnlyWhitespace)
{
    // when
    const bool loaded = cardListLoad(TEST_DATA_DIR "/whitespace_only.txt", &list);

    // then
    EXPECT_THAT(loaded, IsFalse());
}

TEST_F(CardListFile, LoadReturnsFalseWhenFileIsMissing)
{
    // when
    const bool loaded = cardListLoad(TEST_DATA_DIR "/missing.txt", &list);

    // then
    EXPECT_THAT(loaded, IsFalse());
}
} // namespace test
