#pragma once

/*
 * Force-included (-include) before every line of Pathfinder.c in the test build.
 * <stdlib.h> is included first, so its own malloc/calloc/free declarations stay untouched,
 * and its include guard prevents re-parsing when Pathfinder.c includes it again.
 */
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

void* pathfinderMalloc(size_t size);
void* pathfinderCalloc(size_t count, size_t size);
void pathfinderFree(void* pointer);

#ifdef __cplusplus
}
#endif

#define malloc(size) pathfinderMalloc(size)
#define calloc(count, size) pathfinderCalloc(count, size)
#define free(pointer) pathfinderFree(pointer)
