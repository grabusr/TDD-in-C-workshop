#include "Game.h"

#define BLACKJACK 21
#define DEALER_STAND_VALUE 17
#define CARDS_IN_INITIAL_DEAL 4u

static bool deckHasCards(const Game* const game, const size_t count)
{
    return game->deck->count - game->nextCard >= count;
}

static Card takeCard(Game* const game)
{
    return game->deck->cards[game->nextCard++];
}

static void dealTo(Game* const game, Hand* const hand)
{
    handAddCard(hand, takeCard(game));
}

static RoundState endGameAsVoid(Game* const game)
{
    game->over = true;
    return ROUND_VOID;
}

static RoundState settle(Game* const game)
{
    if (handIsBust(&game->dealer)) {
        game->score.playerWins++;
        return ROUND_PLAYER_WON;
    }

    const int playerValue = handValue(&game->player);
    const int dealerValue = handValue(&game->dealer);

    if (playerValue > dealerValue) {
        game->score.playerWins++;
        return ROUND_PLAYER_WON;
    }
    if (playerValue < dealerValue) {
        game->score.dealerWins++;
        return ROUND_DEALER_WON;
    }

    game->score.pushes++;
    return ROUND_PUSH;
}

static RoundState playDealerTurn(Game* const game)
{
    while (handValue(&game->dealer) < DEALER_STAND_VALUE) {
        if (!deckHasCards(game, 1)) {
            return endGameAsVoid(game);
        }
        dealTo(game, &game->dealer);
    }

    return settle(game);
}

void gameInit(Game* const game, const CardList* const deck)
{
    *game = (Game){0};
    game->deck = deck;
}

RoundState gameStartRound(Game* const game)
{
    if (game->over || !deckHasCards(game, CARDS_IN_INITIAL_DEAL)) {
        return endGameAsVoid(game);
    }

    game->player = (Hand){0};
    game->dealer = (Hand){0};

    dealTo(game, &game->player);
    dealTo(game, &game->dealer);
    dealTo(game, &game->player);
    dealTo(game, &game->dealer);

    return ROUND_AWAITING_DECISION;
}

RoundState gameApplyDecision(Game* const game, const Decision decision)
{
    if (game->over) {
        return ROUND_VOID;
    }

    if (decision == DECISION_STAND) {
        return playDealerTurn(game);
    }

    if (!deckHasCards(game, 1)) {
        return endGameAsVoid(game);
    }

    dealTo(game, &game->player);
    if (handIsBust(&game->player)) {
        game->score.dealerWins++;
        return ROUND_DEALER_WON;
    }

    return ROUND_AWAITING_DECISION;
}

const Hand* gamePlayerHand(const Game* const game)
{
    return &game->player;
}

const Hand* gameDealerHand(const Game* const game)
{
    return &game->dealer;
}

Card gameDealerUpCard(const Game* const game)
{
    return game->dealer.cards[0];
}

Score gameScore(const Game* const game)
{
    return game->score;
}

bool gameIsOver(const Game* const game)
{
    return game->over;
}
