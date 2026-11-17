#pragma once

#include <Logger.h>
#include <PlayerStrategy.h>

#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /// Strategy asking a human: prompts through `output` and reads the answer line from `input`.
    /// `h` hits, `s` stands (case and surrounding white space do not matter), anything else is asked again. When the
    /// input ends the player stands. Do not copy the struct: `strategy.context` points to it.
    typedef struct ConsolePlayerStrategy
    {
        PlayerStrategy strategy;
        FILE* input;
        const Logger* output;
    } ConsolePlayerStrategy;

    void consolePlayerStrategyInit(ConsolePlayerStrategy* self, FILE* input, const Logger* output);

#ifdef __cplusplus
}
#endif
