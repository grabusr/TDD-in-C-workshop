#include "CardShuffler.h"

void cardListShuffle(CardList* const list, const RandomGenerator* const generator)
{
    if (list == NULL || generator == NULL || list->count < 2) {
        return;
    }

    const size_t last = list->count - 1;
    for (size_t current = 0; current <= last; ++current) {
        const size_t drawn = generator->next(generator->context, 0, last);
        if (drawn != current) {
            const Card swapped = list->cards[current];
            list->cards[current] = list->cards[drawn];
            list->cards[drawn] = swapped;
        }
    }
}
