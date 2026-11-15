#pragma once

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct Coordinate
{
    int x;
    int y;
} Coordinate;

typedef unsigned int MapCost;

enum
{
    mapImpassableCost = 0,
    mapDefaultCost = 1
};

typedef struct Map Map;

Map* mapCreate(size_t width, size_t height);
void mapDestroy(Map* map);

size_t mapGetWidth(const Map* map);
size_t mapGetHeight(const Map* map);

bool mapContains(const Map* map, Coordinate coordinate);
bool mapSetCost(Map* map, Coordinate coordinate, MapCost cost);
MapCost mapGetCost(const Map* map, Coordinate coordinate);
bool mapIsWalkable(const Map* map, Coordinate coordinate);

#ifdef __cplusplus
}
#endif
