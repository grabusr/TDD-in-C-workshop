#pragma once

#include "Card.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/// 22 cards is the longest hand that does not bust: 4 aces, 4 twos and 3 threes sum to 21.
#define HAND_MAX_CARDS 22u

    typedef struct Hand
    {
        Card cards[HAND_MAX_CARDS];
        size_t count;
    } Hand;

    /// Returns blackjack value of the hand: aces count as 11 when it does not exceed 21, otherwise as 1.
    /// Only first `count` cards are taken into account. Empty hand has value 0.
    int handValue(const Hand* hand);

    /// Appends the card to the hand. Returns false and leaves the hand untouched when the hand is full.
    bool handAddCard(Hand* hand, Card card);

    /// Returns true when the hand value exceeds 21.
    bool handIsBust(const Hand* hand);

#ifdef __cplusplus
}
#endif
