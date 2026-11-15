#pragma once

/*
 * Force-included (-include) before every line of Map.c in the test build.
 * <stdlib.h> is included first, so its own malloc/calloc/free declarations stay untouched,
 * and its include guard prevents re-parsing when Map.c includes it again.
 */
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

void* mapMalloc(size_t size);
void* mapCalloc(size_t count, size_t size);
void mapFree(void* pointer);

#ifdef __cplusplus
}
#endif

#define malloc(size) mapMalloc(size)
#define calloc(count, size) mapCalloc(count, size)
#define free(pointer) mapFree(pointer)
