#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct RandomGenerator
    {
        /// Returns random value from the closed range [min, max].
        size_t (*next)(void* context, size_t min, size_t max);
        void* context;
    } RandomGenerator;

#ifdef __cplusplus
}
#endif
