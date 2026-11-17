#include "CardList.h"

#include <stdlib.h>
#include <string.h>

const static char* const separators = " \t\r\n";

static bool parseRank(const char* const token, Rank* const out)
{
    const static char* const names[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    for (size_t i = 0; i < sizeof(names) / sizeof(names[0]); ++i) {
        if (strcmp(token, names[i]) == 0) {
            *out = (Rank)(RANK_ACE + i);
            return true;
        }
    }
    return false;
}

CardList createCardListFromString(const char* const source)
{
    const CardList empty = {NULL, 0};

    if (source == NULL) {
        return empty;
    }

    const size_t length = strlen(source);
    char* const text = malloc(length + 1);
    if (text == NULL) {
        return empty;
    }
    memcpy(text, source, length + 1);

    size_t tokens = 0;
    for (const char* cursor = text + strspn(text, separators); *cursor != '\0'; cursor += strspn(cursor, separators)) {
        ++tokens;
        cursor += strcspn(cursor, separators);
    }
    if (tokens == 0) {
        free(text);
        return empty;
    }

    Card* const cards = malloc(tokens * sizeof(Card));
    if (cards == NULL) {
        free(text);
        return empty;
    }

    size_t count = 0;
    char* savePointer = NULL;
    for (char* token = strtok_r(text, separators, &savePointer); token != NULL;
         token = strtok_r(NULL, separators, &savePointer))
    {
        if (!parseRank(token, &cards[count].rank)) {
            free(cards);
            free(text);
            return empty;
        }
        ++count;
    }
    free(text);

    return (CardList){cards, count};
}

void cardListFree(CardList* const list)
{
    free(list->cards);
    list->cards = NULL;
    list->count = 0;
}
