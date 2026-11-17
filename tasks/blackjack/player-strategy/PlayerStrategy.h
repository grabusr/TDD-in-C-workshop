#pragma once

#include <Decision.h>
#include <Hand.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /// Player brain: decides whether to hit or stand and is told about every card it can see, so it is able to count
    /// cards.
    typedef struct PlayerStrategy
    {
        /// Called when the player has to decide, with own hand and the dealer up card.
        Decision (*decide)(void* context, const Hand* playerHand, Card dealerUpCard);
        /// Called for cards drawn by the player and by the dealer, in the order they become visible.
        void (*onCardDrawn)(void* context, Participant who, Card card);
        void* context;
    } PlayerStrategy;

#ifdef __cplusplus
}
#endif
