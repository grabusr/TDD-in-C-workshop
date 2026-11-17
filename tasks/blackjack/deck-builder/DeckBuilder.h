#pragma once

#include <CardList.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /// Builds a list of `deckCount` standard 52-card decks (four cards of each of 13 ranks per deck), not shuffled.
    /// Returns an empty list when `deckCount` is zero or memory cannot be allocated. Free with cardListFree().
    CardList deckBuild(size_t deckCount);

#ifdef __cplusplus
}
#endif
