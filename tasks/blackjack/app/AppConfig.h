#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum PlayerKind
    {
        PLAYER_CAUTIOUS,
        PLAYER_RISKY,
        PLAYER_CUSTOM
    } PlayerKind;

    typedef struct AppConfig
    {
        PlayerKind player;
        /// Hand value from which the player stands. For PLAYER_CUSTOM always in range [1, 21].
        int threshold;
        size_t deckCount;
        bool hasSeed;
        uint64_t seed;
    } AppConfig;

    /// Parses optional `--player=cautious|risky` (ready-made player with threshold 12 or 17), `--player-threshold=N`
    /// (custom player standing from N, clamped to [1, 21]), `--decks=N` (default 1, at least 1) and `--seed=N`.
    /// Without any arguments the cautious player is used. The two player flags exclude each other. `argv[0]` is the
    /// program name. Returns false for any conflicting, unknown or malformed argument; `out` is then unspecified.
    bool appConfigParse(int argc, const char* const* argv, AppConfig* out);

#ifdef __cplusplus
}
#endif
