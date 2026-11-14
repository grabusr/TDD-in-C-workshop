#pragma once

#include <Map.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct Path
{
    Coordinate* points;
    size_t length;
} Path;

Path pathfinderFindPath(const Map* map, Coordinate start, Coordinate goal);
bool pathIsEmpty(const Path* path);
void pathDestroy(Path* path);

#ifdef __cplusplus
}
#endif
