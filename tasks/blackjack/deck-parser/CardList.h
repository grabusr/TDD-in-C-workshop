#pragma once

#include <Card.h>

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CardList
{
    Card* cards;
    size_t count;
} CardList;

/// Loads cards from a text file with whitespace separated ranks: A 2 3 4 5 6 7 8 9 10 J Q K.
/// Returns false when file cannot be opened, is empty or contains unknown rank.
bool cardListLoad(const char* path, CardList* out);

/// Releases memory of loaded list. Safe to call on zero initialized list.
void cardListFree(CardList* list);

#ifdef __cplusplus
}
#endif
