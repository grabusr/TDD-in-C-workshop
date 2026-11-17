#pragma once

#include "Card.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /// Short name of the rank as used in card lists: "A", "2" ... "10", "J", "Q", "K". "?" for an unknown rank.
    const char* cardRankName(Rank rank);

#ifdef __cplusplus
}
#endif
