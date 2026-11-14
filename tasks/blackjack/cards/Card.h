#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum Rank
{
    RANK_ACE = 1,
    RANK_TWO,
    RANK_THREE,
    RANK_FOUR,
    RANK_FIVE,
    RANK_SIX,
    RANK_SEVEN,
    RANK_EIGHT,
    RANK_NINE,
    RANK_TEN,
    RANK_JACK,
    RANK_QUEEN,
    RANK_KING,
} Rank;

typedef struct Card
{
    Rank rank;
} Card;

#ifdef __cplusplus
}
#endif
