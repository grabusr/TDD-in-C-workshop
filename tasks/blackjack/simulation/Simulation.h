#pragma once

#include <CardList.h>
#include <Game.h>
#include <Logger.h>
#include <PlayerStrategy.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /// Plays rounds from the (already shuffled) deck until it runs out of cards. Decisions come from the strategy,
    /// rules are applied by the game model. Describes the course of the game (dealt hands, decisions, round results)
    /// to the logger, which may be NULL to log nothing. Returns the final score.
    Score simulationRun(const CardList* deck, const PlayerStrategy* strategy, const Logger* logger);

#ifdef __cplusplus
}
#endif
