#include "Map.h"

#include <stdlib.h>

struct Map
{
    size_t width;
    size_t height;
    MapCost* costs;
};

static size_t mapIndex(const Map* const map, const Coordinate coordinate)
{
    return (size_t)coordinate.y * map->width + (size_t)coordinate.x;
}

Map* mapCreate(const size_t width, const size_t height)
{
    if (width == 0 || height == 0) {
        return NULL;
    }

    Map* const map = malloc(sizeof(Map));
    if (map == NULL) {
        return NULL;
    }

    const size_t fieldCount = width * height;
    map->costs = malloc(fieldCount * sizeof(MapCost));
    if (map->costs == NULL) {
        free(map);
        return NULL;
    }

    map->width = width;
    map->height = height;
    for (size_t i = 0; i < fieldCount; ++i) {
        map->costs[i] = mapDefaultCost;
    }

    return map;
}

void mapDestroy(Map* const map)
{
    if (map == NULL) {
        return;
    }

    free(map->costs);
    free(map);
}

size_t mapGetWidth(const Map* const map)
{
    return map->width;
}

size_t mapGetHeight(const Map* const map)
{
    return map->height;
}

bool mapContains(const Map* const map, const Coordinate coordinate)
{
    return coordinate.x >= 0 && coordinate.y >= 0 && (size_t)coordinate.x < map->width
        && (size_t)coordinate.y < map->height;
}

bool mapSetCost(Map* const map, const Coordinate coordinate, const MapCost cost)
{
    if (!mapContains(map, coordinate)) {
        return false;
    }

    map->costs[mapIndex(map, coordinate)] = cost;
    return true;
}

MapCost mapGetCost(const Map* const map, const Coordinate coordinate)
{
    if (!mapContains(map, coordinate)) {
        return mapImpassableCost;
    }

    return map->costs[mapIndex(map, coordinate)];
}

bool mapIsWalkable(const Map* const map, const Coordinate coordinate)
{
    return mapGetCost(map, coordinate) != mapImpassableCost;
}
