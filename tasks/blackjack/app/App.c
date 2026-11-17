#include "App.h"

#include <CardShuffler.h>
#include <DeckBuilder.h>
#include <SeededRandom.h>
#include <Simulation.h>
#include <ThresholdPlayerStrategy.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define LOG_BUFFER_SIZE 128u

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

static const char* playerName(const PlayerKind player)
{
    switch (player) {
        case PLAYER_CAUTIOUS: return "cautious";
        case PLAYER_RISKY: return "risky";
        case PLAYER_CUSTOM: return "custom";
    }
    return "unknown";
}

bool appPlay(const AppConfig* const config, const uint64_t seed, const Logger* const logger, Score* const score)
{
    CardList deck = deckBuild(config->deckCount);
    if (deck.cards == NULL) {
        return false;
    }

    SeededRandom random;
    seededRandomInit(&random, seed);
    cardListShuffle(&deck, &random.generator);

    ThresholdPlayerStrategy strategy;
    thresholdPlayerStrategyInit(&strategy, config->threshold);

    *score = simulationRun(&deck, &strategy.strategy, logger);
    cardListFree(&deck);
    return true;
}

int appRun(const AppConfig* const config, const uint64_t fallbackSeed, const Logger* const logger)
{
    const uint64_t seed = config->hasSeed ? config->seed : fallbackSeed;

    logMessage(logger, "player: %s", playerName(config->player));
    logMessage(logger, "threshold: %d", config->threshold);
    logMessage(logger, "decks: %zu", config->deckCount);
    logMessage(logger, "seed: %llu", (unsigned long long)seed);

    Score score;
    if (!appPlay(config, seed, logger, &score)) {
        logMessage(logger, "Cannot create the deck");
        return EXIT_FAILURE;
    }

    logMessage(logger, "player wins: %zu", score.playerWins);
    logMessage(logger, "dealer wins: %zu", score.dealerWins);
    logMessage(logger, "pushes: %zu", score.pushes);
    return EXIT_SUCCESS;
}
