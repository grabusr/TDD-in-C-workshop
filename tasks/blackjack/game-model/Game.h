#pragma once

#include "Decision.h"

#include <CardList.h>
#include <Hand.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct Score
    {
        size_t playerWins;
        size_t dealerWins;
        size_t pushes;
    } Score;

    typedef enum RoundState
    {
        ROUND_AWAITING_DECISION,
        ROUND_PLAYER_WON,
        ROUND_DEALER_WON,
        ROUND_PUSH,
        ROUND_VOID,
    } RoundState;

    /// Game does not own the deck. The deck must outlive the game.
    typedef struct Game
    {
        const CardList* deck;
        size_t nextCard;
        Hand player;
        Hand dealer;
        Score score;
        bool over;
    } Game;

    /// Starts new game: resets the score.
    void gameInit(Game* game, const CardList* deck);

    /// Deals two cards each (player, dealer, player, dealer). Returns ROUND_VOID and ends the game when the deck
    /// runs out of cards, the score does not change then.
    RoundState gameStartRound(Game* game);

    /// Applies player decision. Returns ROUND_AWAITING_DECISION while the player may decide again, otherwise round
    /// result (score is updated) or ROUND_VOID when the deck runs out of cards (score does not change).
    RoundState gameApplyDecision(Game* game, Decision decision);

    const Hand* gamePlayerHand(const Game* game);
    /// Dealer hand: first card is the up card, the others become known when the player stands.
    const Hand* gameDealerHand(const Game* game);
    Card gameDealerUpCard(const Game* game);
    Score gameScore(const Game* game);
    bool gameIsOver(const Game* game);

#ifdef __cplusplus
}
#endif
