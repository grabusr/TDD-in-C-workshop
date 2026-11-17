#include "DeckBuilder.h"

#include <stdint.h>
#include <stdlib.h>

#define CARDS_PER_DECK 52u
#define SUITS_COUNT 4u

CardList deckBuild(const size_t deckCount)
{
    const CardList empty = {NULL, 0};
    if (deckCount == 0u || deckCount > SIZE_MAX / CARDS_PER_DECK / sizeof(Card))
    {
        return empty;
    }

    const size_t count = deckCount * CARDS_PER_DECK;
    Card* const cards = malloc(count * sizeof(Card));
    if (cards == NULL)
    {
        return empty;
    }

    size_t index = 0;
    for (size_t copy = 0; copy < deckCount * SUITS_COUNT; ++copy)
    {
        for (int rank = RANK_ACE; rank <= RANK_KING; ++rank)
        {
            cards[index++] = (Card){.rank = (Rank)rank};
        }
    }
    return (CardList){.cards = cards, .count = count};
}
