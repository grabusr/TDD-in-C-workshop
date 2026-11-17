#pragma once
#include <cstddef>

typedef struct RandomGenerator
{
    size_t (*getRandomNumber)(void* context);
};

void shuffleCards(CardsList cardList, RandomGenerator randomGenerator);