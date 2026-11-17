#include "Hand.h"

#include <stdbool.h>

static int cardValue(const Rank rank)
{
    return rank > RANK_TEN ? 10 : (int)rank;
}

int handValue(const Hand* const hand)
{
    int sum = 0;
    bool hasAce = false;

    for (size_t i = 0; i < hand->count; ++i) {
        const Rank rank = hand->cards[i].rank;
        sum += cardValue(rank);
        hasAce = hasAce || rank == RANK_ACE;
    }

    return (hasAce && sum + 10 <= 21) ? sum + 10 : sum;
}

bool handAddCard(Hand* const hand, const Card card)
{
    if (hand->count >= HAND_MAX_CARDS) {
        return false;
    }

    hand->cards[hand->count++] = card;
    return true;
}

bool handIsBust(const Hand* const hand)
{
    return handValue(hand) > 21;
}
