#pragma once

#include <CardList.h>
#include <RandomGenerator.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /// Shuffles cards in place. Each card, one after another, is swapped with the card at index drawn from
    /// generator in range [0, count - 1]. When drawn index equals the current one, nothing is swapped.
    /// Generator is not used for lists with less than two cards. Does nothing when list or generator is NULL.
    void cardListShuffle(CardList* list, const RandomGenerator* generator);

#ifdef __cplusplus
}
#endif
