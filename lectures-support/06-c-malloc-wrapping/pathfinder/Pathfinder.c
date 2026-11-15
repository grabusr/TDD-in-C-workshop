#include "Pathfinder.h"

#include <limits.h>
#include <stdlib.h>

typedef unsigned long PathCost;

enum
{
    directionCount = 4
};

const static Coordinate directions[directionCount] = {
    { 1,  0},
    {-1,  0},
    { 0,  1},
    { 0, -1},
};

typedef struct SearchState
{
    size_t width;
    size_t fieldCount;
    Coordinate goal;
    PathCost* gScore;
    size_t* cameFrom;
    bool* open;
    bool* closed;
} SearchState;

static Path emptyPath(void)
{
    const Path path = {NULL, 0};
    return path;
}

static size_t toIndex(const SearchState* const state, const Coordinate coordinate)
{
    return (size_t)coordinate.y * state->width + (size_t)coordinate.x;
}

static Coordinate toCoordinate(const SearchState* const state, const size_t index)
{
    const Coordinate coordinate = {(int)(index % state->width), (int)(index / state->width)};
    return coordinate;
}

static PathCost calculateHeuristic(const Coordinate from, const Coordinate to)
{
    return (PathCost)abs(from.x - to.x) + (PathCost)abs(from.y - to.y);
}

static void searchStateDestroy(SearchState* const state)
{
    free(state->gScore);
    free(state->cameFrom);
    free(state->open);
    free(state->closed);
}

static bool searchStateCreate(SearchState* const state, const Map* const map, const Coordinate goal)
{
    state->width = mapGetWidth(map);
    state->fieldCount = state->width * mapGetHeight(map);
    state->goal = goal;
    state->gScore = malloc(state->fieldCount * sizeof(PathCost));
    state->cameFrom = malloc(state->fieldCount * sizeof(size_t));
    state->open = calloc(state->fieldCount, sizeof(bool));
    state->closed = calloc(state->fieldCount, sizeof(bool));

    if (state->gScore == NULL || state->cameFrom == NULL || state->open == NULL || state->closed == NULL) {
        searchStateDestroy(state);
        return false;
    }

    for (size_t i = 0; i < state->fieldCount; ++i) {
        state->gScore[i] = ULONG_MAX;
    }
    return true;
}

/* Returns index of the open field with the lowest f = g + h, or fieldCount when the open set is empty. */
static size_t popLowestScoreField(const SearchState* const state)
{
    size_t best = state->fieldCount;
    PathCost bestScore = ULONG_MAX;

    for (size_t i = 0; i < state->fieldCount; ++i) {
        if (!state->open[i]) {
            continue;
        }

        const PathCost score = state->gScore[i] + calculateHeuristic(toCoordinate(state, i), state->goal);
        if (score < bestScore) {
            bestScore = score;
            best = i;
        }
    }
    return best;
}

static void relaxNeighbours(SearchState* const state, const Map* const map, const size_t current)
{
    const Coordinate position = toCoordinate(state, current);

    for (size_t d = 0; d < directionCount; ++d) {
        const Coordinate neighbour = {position.x + directions[d].x, position.y + directions[d].y};
        if (!mapIsWalkable(map, neighbour)) {
            continue;
        }

        const size_t index = toIndex(state, neighbour);
        if (state->closed[index]) {
            continue;
        }

        const PathCost tentative = state->gScore[current] + mapGetCost(map, neighbour);
        if (tentative < state->gScore[index]) {
            state->gScore[index] = tentative;
            state->cameFrom[index] = current;
            state->open[index] = true;
        }
    }
}

static Path reconstructPath(const SearchState* const state, const size_t startIndex, const size_t goalIndex)
{
    size_t length = 1;
    for (size_t i = goalIndex; i != startIndex; i = state->cameFrom[i]) {
        ++length;
    }

    Path path = {malloc(length * sizeof(Coordinate)), length};
    if (path.points == NULL) {
        return emptyPath();
    }

    size_t index = goalIndex;
    for (size_t i = length; i > 0; --i) {
        path.points[i - 1] = toCoordinate(state, index);
        index = state->cameFrom[index];
    }
    return path;
}

Path pathfinderFindPath(const Map* const map, const Coordinate start, const Coordinate goal)
{
    if (map == NULL || !mapIsWalkable(map, start) || !mapIsWalkable(map, goal)) {
        return emptyPath();
    }

    SearchState state;
    if (!searchStateCreate(&state, map, goal)) {
        return emptyPath();
    }

    const size_t startIndex = toIndex(&state, start);
    const size_t goalIndex = toIndex(&state, goal);
    state.gScore[startIndex] = 0;
    state.open[startIndex] = true;

    Path path = emptyPath();
    for (size_t current = popLowestScoreField(&state); current != state.fieldCount;
         current = popLowestScoreField(&state))
    {
        if (current == goalIndex) {
            path = reconstructPath(&state, startIndex, goalIndex);
            break;
        }

        state.open[current] = false;
        state.closed[current] = true;
        relaxNeighbours(&state, map, current);
    }

    searchStateDestroy(&state);
    return path;
}

bool pathIsEmpty(const Path* const path)
{
    return path->length == 0;
}

void pathDestroy(Path* const path)
{
    if (path == NULL) {
        return;
    }

    free(path->points);
    path->points = NULL;
    path->length = 0;
}
