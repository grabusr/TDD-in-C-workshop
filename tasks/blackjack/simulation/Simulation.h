#pragma once

#include <CardList.h>
#include <Game.h>
#include <PlayerStrategy.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /// Plays rounds from the (already shuffled) deck until it runs out of cards. Decisions come from the strategy,
    /// rules are applied by the game model. Returns the final score.
    Score simulationRun(const CardList* deck, const PlayerStrategy* strategy);

#ifdef __cplusplus
}
#endif
