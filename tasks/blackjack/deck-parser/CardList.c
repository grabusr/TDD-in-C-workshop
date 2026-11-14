#include "CardList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    chunkSize = 256
};

static const char* const separators = " \t\r\n";

static bool parseRank(const char* const token, Rank* const out)
{
    static const char* const names[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    for (size_t i = 0; i < sizeof(names) / sizeof(names[0]); ++i)
    {
        if (strcmp(token, names[i]) == 0)
        {
            *out = (Rank)(RANK_ACE + i);
            return true;
        }
    }
    return false;
}

bool cardListLoad(const char* const path, CardList* const out)
{
    FILE* const file = fopen(path, "r");
    if (file == NULL)
    {
        return false;
    }

    char* text = NULL;
    size_t length = 0;
    char chunk[chunkSize];
    size_t read = 0;
    while ((read = fread(chunk, 1, sizeof(chunk), file)) > 0)
    {
        char* const grown = realloc(text, length + read + 1);
        if (grown == NULL)
        {
            free(text);
            fclose(file);
            return false;
        }
        text = grown;
        memcpy(text + length, chunk, read);
        length += read;
        text[length] = '\0';
    }
    fclose(file);

    if (text == NULL)
    {
        return false;
    }

    size_t tokens = 0;
    for (const char* cursor = text + strspn(text, separators); *cursor != '\0';
         cursor += strspn(cursor, separators))
    {
        ++tokens;
        cursor += strcspn(cursor, separators);
    }
    if (tokens == 0)
    {
        free(text);
        return false;
    }

    Card* const cards = malloc(tokens * sizeof(Card));
    if (cards == NULL)
    {
        free(text);
        return false;
    }

    size_t count = 0;
    char* savePointer = NULL;
    for (char* token = strtok_r(text, separators, &savePointer); token != NULL;
         token = strtok_r(NULL, separators, &savePointer))
    {
        if (!parseRank(token, &cards[count].rank))
        {
            free(cards);
            free(text);
            return false;
        }
        ++count;
    }
    free(text);

    out->cards = cards;
    out->count = count;
    return true;
}

void cardListFree(CardList* const list)
{
    free(list->cards);
    list->cards = NULL;
    list->count = 0;
}
