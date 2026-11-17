#include "ConsolePlayerStrategy.h"

#include <CardName.h>

#include <ctype.h>
#include <string.h>

#define MESSAGE_SIZE 256u
#define ANSWER_SIZE 32u

static void say(const ConsolePlayerStrategy* const self, const char* const message)
{
    self->output->log(self->output->context, message);
}

static void formatPrompt(char* const out, const size_t size, const Hand* const hand, const Card dealerUpCard)
{
    size_t length = (size_t)snprintf(out, size, "Your hand: ");
    for (size_t i = 0; i < hand->count && length < size; ++i)
    {
        length += (size_t)snprintf(out + length, size - length, "%s ", cardRankName(hand->cards[i].rank));
    }
    if (length < size)
    {
        snprintf(out + length, size - length, "(%d), dealer shows %s. [h]it or [s]tand?", handValue(hand),
                 cardRankName(dealerUpCard.rank));
    }
}

/// Returns the first non-space character of the answer, or 0 when the input ended.
static int readAnswer(FILE* const input)
{
    char line[ANSWER_SIZE];
    if (fgets(line, sizeof(line), input) == NULL)
    {
        return 0;
    }
    const char* answer = line;
    while (*answer != '\0' && isspace((unsigned char)*answer))
    {
        ++answer;
    }
    const char* end = answer + strlen(answer);
    while (end > answer && isspace((unsigned char)end[-1]))
    {
        --end;
    }
    if (end - answer != 1)
    {
        return '?';
    }
    return tolower((unsigned char)*answer);
}

static Decision decide(void* context, const Hand* playerHand, Card dealerUpCard)
{
    const ConsolePlayerStrategy* const self = context;
    char prompt[MESSAGE_SIZE];
    formatPrompt(prompt, sizeof(prompt), playerHand, dealerUpCard);
    say(self, prompt);

    for (;;)
    {
        const int answer = readAnswer(self->input);
        if (answer == 'h')
        {
            return DECISION_HIT;
        }
        if (answer == 's' || answer == 0)
        {
            return DECISION_STAND;
        }
        say(self, "Unknown answer, type h to hit or s to stand");
        say(self, prompt);
    }
}

static void onCardDrawn(void* context, Participant who, Card card)
{
    (void)context;
    (void)who;
    (void)card;
}

void consolePlayerStrategyInit(ConsolePlayerStrategy* const self, FILE* const input, const Logger* const output)
{
    self->input = input;
    self->output = output;
    self->strategy.decide = decide;
    self->strategy.onCardDrawn = onCardDrawn;
    self->strategy.context = self;
}
