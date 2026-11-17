#include "CardName.h"

const char* cardRankName(const Rank rank)
{
    switch (rank) {
        case RANK_ACE: return "A";
        case RANK_TWO: return "2";
        case RANK_THREE: return "3";
        case RANK_FOUR: return "4";
        case RANK_FIVE: return "5";
        case RANK_SIX: return "6";
        case RANK_SEVEN: return "7";
        case RANK_EIGHT: return "8";
        case RANK_NINE: return "9";
        case RANK_TEN: return "10";
        case RANK_JACK: return "J";
        case RANK_QUEEN: return "Q";
        case RANK_KING: return "K";
    }
    return "?";
}
