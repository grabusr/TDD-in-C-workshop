#pragma once

#include <RandomGenerator.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /// Deterministic pseudo random generator (splitmix64). The same seed always produces the same sequence,
    /// on every platform. `generator.context` points to the instance, so the instance must not be copied or moved
    /// after initialization.
    typedef struct SeededRandom
    {
        RandomGenerator generator;
        uint64_t state;
    } SeededRandom;

    void seededRandomInit(SeededRandom* self, uint64_t seed);

#ifdef __cplusplus
}
#endif
