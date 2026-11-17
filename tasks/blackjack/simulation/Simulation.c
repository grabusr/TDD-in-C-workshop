#include "Simulation.h"

#include <CardName.h>

#include <stdarg.h>
#include <stdio.h>

#define LOG_BUFFER_SIZE 256u
#define HAND_TEXT_SIZE 96u

static void logMessage(const Logger* const logger, const char* const format, ...)
{
    if (logger == NULL) {
        return;
    }

    char message[LOG_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    logger->log(logger->context, message);
}

/// Writes cards of the hand separated with spaces, e.g. "10 A 7". Longer hands are cut off, not overflowed.
static void formatHand(char* const text, const size_t size, const Hand* const hand)
{
    size_t length = 0;
    text[0] = '\0';

    for (size_t i = 0; i < hand->count && length < size; ++i) {
        const int written = snprintf(text + length, size - length, i == 0 ? "%s" : " %s", cardRankName(hand->cards[i].rank));
        if (written < 0) {
            return;
        }
        length += (size_t)written;
    }
}

static size_t completedRounds(const Game* const game)
{
    const Score score = gameScore(game);
    return score.playerWins + score.dealerWins + score.pushes;
}

static const char* describeResult(const RoundState state)
{
    switch (state) {
        case ROUND_PLAYER_WON: return "player wins";
        case ROUND_DEALER_WON: return "dealer wins";
        default: return "push";
    }
}

static void logRoundStart(const Game* const game, const Logger* const logger)
{
    char playerText[HAND_TEXT_SIZE];
    formatHand(playerText, sizeof(playerText), gamePlayerHand(game));

    logMessage(logger, "Round %zu: player has %s (%d), dealer shows %s", completedRounds(game) + 1, playerText,
               handValue(gamePlayerHand(game)), cardRankName(gameDealerUpCard(game).rank));
}

static void logPlayerHit(const Game* const game, const Logger* const logger)
{
    const Hand* const player = gamePlayerHand(game);

    logMessage(logger, "Player hits, draws %s (hand value %d)", cardRankName(player->cards[player->count - 1].rank),
               handValue(player));
}

static void logDealerTurn(const Game* const game, const Logger* const logger)
{
    char dealerText[HAND_TEXT_SIZE];
    formatHand(dealerText, sizeof(dealerText), gameDealerHand(game));

    logMessage(logger, "Dealer plays: %s (hand value %d)", dealerText, handValue(gameDealerHand(game)));
}

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

static RoundState playRound(Game* const game, const PlayerStrategy* const strategy, const Logger* const logger)
{
    RoundState state = gameStartRound(game);
    if (state != ROUND_AWAITING_DECISION) {
        return state;
    }
    informAboutDealtCards(game, strategy);
    logRoundStart(game, logger);

    while (state == ROUND_AWAITING_DECISION) {
        const Decision decision = strategy->decide(strategy->context, gamePlayerHand(game), gameDealerUpCard(game));
        if (decision == DECISION_STAND) {
            logMessage(logger, "Player stands with %d", handValue(gamePlayerHand(game)));
        }
        state = gameApplyDecision(game, decision);

        if (decision == DECISION_STAND) {
            informAboutDealerTurn(game, strategy);
            if (state != ROUND_VOID) {
                logDealerTurn(game, logger);
            }
        }
        else if (state != ROUND_VOID) {
            informAboutPlayerHit(game, strategy);
            logPlayerHit(game, logger);
        }
    }

    if (state != ROUND_VOID) {
        logMessage(logger, "Round %zu: %s", completedRounds(game), describeResult(state));
    }
    return state;
}

Score simulationRun(const CardList* const deck, const PlayerStrategy* const strategy, const Logger* const logger)
{
    Game game;
    gameInit(&game, deck);

    while (playRound(&game, strategy, logger) != ROUND_VOID) {}

    logMessage(logger, "Deck exhausted, completed rounds: %zu", completedRounds(&game));
    return gameScore(&game);
}
