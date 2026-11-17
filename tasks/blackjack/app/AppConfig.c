#include "AppConfig.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define CAUTIOUS_THRESHOLD 12
#define RISKY_THRESHOLD 17
#define MIN_THRESHOLD 1
#define MAX_THRESHOLD 21

/// Returns the text after `name=` when `arg` starts with it, otherwise NULL.
static const char* valueOf(const char* const arg, const char* const name)
{
    const size_t length = strlen(name);
    if (strncmp(arg, name, length) != 0 || arg[length] != '=')
    {
        return NULL;
    }
    return arg + length + 1;
}

static bool parseUnsigned(const char* const text, unsigned long long* const out)
{
    if (text[0] < '0' || text[0] > '9')
    {
        return false;
    }
    char* end = NULL;
    errno = 0;
    const unsigned long long value = strtoull(text, &end, 10);
    if (*end != '\0' || errno == ERANGE)
    {
        return false;
    }
    *out = value;
    return true;
}

/// Parses a signed number; values out of the int range saturate, which is fine as they are clamped afterwards.
static bool parseThreshold(const char* const text, int* const out)
{
    const char* digits = text[0] == '-' ? text + 1 : text;
    if (digits[0] < '0' || digits[0] > '9')
    {
        return false;
    }
    char* end = NULL;
    errno = 0;
    const long long value = strtoll(text, &end, 10);
    if (*end != '\0')
    {
        return false;
    }
    if (value < MIN_THRESHOLD)
    {
        *out = MIN_THRESHOLD;
    }
    else if (value > MAX_THRESHOLD)
    {
        *out = MAX_THRESHOLD;
    }
    else
    {
        *out = (int)value;
    }
    return true;
}

bool appConfigParse(const int argc, const char* const* const argv, AppConfig* const out)
{
    AppConfig config = {.player = PLAYER_CAUTIOUS, .deckCount = 1};
    bool hasPlayer = false;
    bool hasThreshold = false;
    int threshold = 0;

    for (int i = 1; i < argc; ++i)
    {
        const char* value = NULL;
        unsigned long long number = 0;
        if ((value = valueOf(argv[i], "--player")) != NULL)
        {
            if (strcmp(value, "cautious") == 0)
            {
                config.player = PLAYER_CAUTIOUS;
            }
            else if (strcmp(value, "risky") == 0)
            {
                config.player = PLAYER_RISKY;
            }
            else
            {
                return false;
            }
            hasPlayer = true;
        }
        else if ((value = valueOf(argv[i], "--player-threshold")) != NULL)
        {
            if (!parseThreshold(value, &threshold))
            {
                return false;
            }
            hasThreshold = true;
        }
        else if ((value = valueOf(argv[i], "--decks")) != NULL)
        {
            if (!parseUnsigned(value, &number) || number == 0u || number > SIZE_MAX)
            {
                return false;
            }
            config.deckCount = (size_t)number;
        }
        else if ((value = valueOf(argv[i], "--seed")) != NULL)
        {
            if (!parseUnsigned(value, &number))
            {
                return false;
            }
            config.hasSeed = true;
            config.seed = number;
        }
        else
        {
            return false;
        }
    }

    if (hasPlayer && hasThreshold) {
        return false;
    }
    if (hasThreshold) {
        config.player = PLAYER_CUSTOM;
        config.threshold = threshold;
    }
    else {
        config.threshold = config.player == PLAYER_CAUTIOUS ? CAUTIOUS_THRESHOLD : RISKY_THRESHOLD;
    }
    *out = config;
    return true;
}
