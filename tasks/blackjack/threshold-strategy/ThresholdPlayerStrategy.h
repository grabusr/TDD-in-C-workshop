#pragma once

#include <PlayerStrategy.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /// Strategy deciding only by the player's hand value: hits below the threshold, stands at or above it.
    typedef struct ThresholdPlayerStrategy
    {
        PlayerStrategy strategy;
        int standThreshold;
    } ThresholdPlayerStrategy;

    void thresholdPlayerStrategyInit(ThresholdPlayerStrategy* self, int standThreshold);

#ifdef __cplusplus
}
#endif
