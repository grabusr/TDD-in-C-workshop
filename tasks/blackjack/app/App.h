#pragma once

#include "AppConfig.h"

#include <Game.h>
#include <Logger.h>

#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /// Builds `config->deckCount` decks, shuffles them with a generator seeded with `seed` and plays the whole shoe with
    /// the strategy chosen in `config`. The same config and seed always give the same score. The course of the game
    /// is reported to `logger`, which may be NULL. The console player asks its questions through `logger` and reads
    /// the answers from `input`. Returns false when the deck cannot be created.
    bool appPlay(const AppConfig* config, uint64_t seed, FILE* input, const Logger* logger, Score* score);

    /// Plays like appPlay() using `config->seed` when set, `fallbackSeed` otherwise. Logs the used parameters
    /// (including the seed) first, then the course of the game and finally the score. Returns the process exit code.
    int appRun(const AppConfig* config, uint64_t fallbackSeed, FILE* input, const Logger* logger);

    /// Describes the command line flags and the console player controls, one `logger` message per line.
    void appPrintUsage(const char* program, const Logger* logger);

#ifdef __cplusplus
}
#endif
