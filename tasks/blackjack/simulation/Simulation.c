#include "Simulation.h"

static void informAboutCard(const PlayerStrategy* const strategy, const Participant who, const Card card)
{
    strategy->onCardDrawn(strategy->context, who, card);
}

static void informAboutDealtCards(const Game* const game, const PlayerStrategy* const strategy)
{
    const Hand* const player = gamePlayerHand(game);

    informAboutCard(strategy, PARTICIPANT_PLAYER, player->cards[0]);
    informAboutCard(strategy, PARTICIPANT_DEALER, gameDealerUpCard(game));
    informAboutCard(strategy, PARTICIPANT_PLAYER, player->cards[1]);
}

static void informAboutPlayerHit(const Game* const game, const PlayerStrategy* const strategy)
{
    const Hand* const player = gamePlayerHand(game);

    informAboutCard(strategy, PARTICIPANT_PLAYER, player->cards[player->count - 1]);
}

/// Dealer cards other than the up card become known when the player stands.
static void informAboutDealerTurn(const Game* const game, const PlayerStrategy* const strategy)
{
    const Hand* const dealer = gameDealerHand(game);

    for (size_t i = 1; i < dealer->count; ++i) {
        informAboutCard(strategy, PARTICIPANT_DEALER, dealer->cards[i]);
    }
}

static RoundState playRound(Game* const game, const PlayerStrategy* const strategy)
{
    RoundState state = gameStartRound(game);
    if (state != ROUND_AWAITING_DECISION) {
        return state;
    }
    informAboutDealtCards(game, strategy);

    while (state == ROUND_AWAITING_DECISION) {
        const Decision decision = strategy->decide(strategy->context, gamePlayerHand(game), gameDealerUpCard(game));
        state = gameApplyDecision(game, decision);

        if (decision == DECISION_STAND) {
            informAboutDealerTurn(game, strategy);
        }
        else if (state != ROUND_VOID) {
            informAboutPlayerHit(game, strategy);
        }
    }

    return state;
}

Score simulationRun(const CardList* const deck, const PlayerStrategy* const strategy)
{
    Game game;
    gameInit(&game, deck);

    while (playRound(&game, strategy) != ROUND_VOID) {}

    return gameScore(&game);
}
