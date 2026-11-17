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

/// Creates card list from text with whitespace separated ranks: A 2 3 4 5 6 7 8 9 10 J Q K.
/// Returns empty list (cards == NULL, count == 0) when text is NULL, empty, contains only whitespace
/// or contains unknown rank. Returned list must be released with cardListFree.
CardList createCardListFromString(const char* text);

/// Releases memory of loaded list. Safe to call on zero initialized list.
void cardListFree(CardList* list);

#ifdef __cplusplus
}
#endif
