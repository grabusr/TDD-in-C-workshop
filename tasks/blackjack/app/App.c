#include "App.h"

#include <CardShuffler.h>
#include <ConsolePlayerStrategy.h>
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
        case PLAYER_CONSOLE: return "console";
        case PLAYER_CAUTIOUS: return "cautious";
        case PLAYER_RISKY: return "risky";
        case PLAYER_CUSTOM: return "custom";
    }
    return "unknown";
}

bool appPlay(const AppConfig* const config, const uint64_t seed, FILE* const input, const Logger* const logger,
             Score* const score)
{
    CardList deck = deckBuild(config->deckCount);
    if (deck.cards == NULL) {
        return false;
    }

    SeededRandom random;
    seededRandomInit(&random, seed);
    cardListShuffle(&deck, &random.generator);

    ConsolePlayerStrategy console;
    ThresholdPlayerStrategy threshold;
    const PlayerStrategy* strategy = &threshold.strategy;
    if (config->player == PLAYER_CONSOLE) {
        consolePlayerStrategyInit(&console, input, logger);
        strategy = &console.strategy;
    }
    else {
        thresholdPlayerStrategyInit(&threshold, config->threshold);
    }

    *score = simulationRun(&deck, strategy, logger);
    cardListFree(&deck);
    return true;
}

int appRun(const AppConfig* const config, const uint64_t fallbackSeed, FILE* const input, const Logger* const logger)
{
    const uint64_t seed = config->hasSeed ? config->seed : fallbackSeed;

    logMessage(logger, "player: %s", playerName(config->player));
    if (config->player != PLAYER_CONSOLE) {
        logMessage(logger, "threshold: %d", config->threshold);
    }
    logMessage(logger, "decks: %zu", config->deckCount);
    logMessage(logger, "seed: %llu", (unsigned long long)seed);

    Score score;
    if (!appPlay(config, seed, input, logger, &score)) {
        logMessage(logger, "Cannot create the deck");
        return EXIT_FAILURE;
    }

    logMessage(logger, "player wins: %zu", score.playerWins);
    logMessage(logger, "dealer wins: %zu", score.dealerWins);
    logMessage(logger, "pushes: %zu", score.pushes);
    return EXIT_SUCCESS;
}

void appPrintUsage(const char* const program, const Logger* const logger)
{
    logMessage(logger, "Usage: %s [--player=console|cautious|risky | --player-threshold=N] [--decks=N] [--seed=N]",
               program);
    logMessage(logger, "       %s --help", program);
    logMessage(logger, "");
    logMessage(logger, "Plays blackjack against the dealer until the shoe runs out.");
    logMessage(logger, "");
    logMessage(logger, "Options:");
    logMessage(logger, "  --player=console|cautious|risky  who plays: you (default), a player hitting up to 11 or");
    logMessage(logger, "                                   one hitting below 17 like the dealer");
    logMessage(logger, "  --player-threshold=N             automatic player standing from hand value N (1-21)");
    logMessage(logger, "  --decks=N                        number of 52-card decks in the shoe (default 1)");
    logMessage(logger, "  --seed=N                         seed of the shuffle, the same seed gives the same game");
    logMessage(logger, "  -h, --help                       show this help and exit");
    logMessage(logger, "");
    logMessage(logger, "Console player controls: h = hit, s = stand, confirmed with Enter.");
}
