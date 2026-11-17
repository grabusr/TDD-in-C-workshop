#include <AppConfig.h>

#include <gmock/gmock.h>

#include <vector>

namespace test
{
using namespace ::testing;

namespace
{
class AppConfigParse : public Test
{
protected:
    void SetUp() override
    {
        args = {"blackjack"};
    }

    bool parse()
    {
        return appConfigParse(static_cast<int>(args.size()), args.data(), &config);
    }

    std::vector<const char*> args;
    AppConfig config{};
};
} // namespace

TEST_F(AppConfigParse, AppConfigParseAcceptsMinimalArguments)
{
    // given
    args.push_back("--player=cautious");

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsTrue());
}

TEST_F(AppConfigParse, AppConfigParseSelectsCautiousPlayer)
{
    // given
    args.push_back("--player=cautious");

    // when
    parse();

    // then
    EXPECT_THAT(config.player, Eq(PLAYER_CAUTIOUS));
}

TEST_F(AppConfigParse, AppConfigParseSelectsRiskyPlayer)
{
    // given
    args.push_back("--player=risky");

    // when
    parse();

    // then
    EXPECT_THAT(config.player, Eq(PLAYER_RISKY));
}

TEST_F(AppConfigParse, AppConfigParseUsesThresholdTwelveForCautiousPlayer)
{
    // given
    args.push_back("--player=cautious");

    // when
    parse();

    // then
    EXPECT_THAT(config.threshold, Eq(12));
}

TEST_F(AppConfigParse, AppConfigParseUsesThresholdSeventeenForRiskyPlayer)
{
    // given
    args.push_back("--player=risky");

    // when
    parse();

    // then
    EXPECT_THAT(config.threshold, Eq(17));
}

TEST_F(AppConfigParse, AppConfigParseSetsThresholdWhenPlayerThresholdGiven)
{
    // given
    args.push_back("--player-threshold=15");

    // when
    parse();

    // then
    EXPECT_THAT(config.threshold, Eq(15));
}

TEST_F(AppConfigParse, AppConfigParseClampsThresholdToTwentyOne)
{
    // given
    args.push_back("--player-threshold=99");

    // when
    parse();

    // then
    EXPECT_THAT(config.threshold, Eq(21));
}

TEST_F(AppConfigParse, AppConfigParseClampsHugeThresholdToTwentyOne)
{
    // given
    args.push_back("--player-threshold=99999999999999999999999");

    // when
    parse();

    // then
    EXPECT_THAT(config.threshold, Eq(21));
}

TEST_F(AppConfigParse, AppConfigParseClampsZeroThresholdToOne)
{
    // given
    args.push_back("--player-threshold=0");

    // when
    parse();

    // then
    EXPECT_THAT(config.threshold, Eq(1));
}

TEST_F(AppConfigParse, AppConfigParseClampsNegativeThresholdToOne)
{
    // given
    args.push_back("--player-threshold=-5");

    // when
    parse();

    // then
    EXPECT_THAT(config.threshold, Eq(1));
}

TEST_F(AppConfigParse, AppConfigParseUsesOneDeckByDefault)
{
    // given
    args.push_back("--player=risky");

    // when
    parse();

    // then
    EXPECT_THAT(config.deckCount, Eq(1u));
}

TEST_F(AppConfigParse, AppConfigParseReadsDeckCount)
{
    // given
    args.insert(args.end(), {"--player=risky", "--decks=6"});

    // when
    parse();

    // then
    EXPECT_THAT(config.deckCount, Eq(6u));
}

TEST_F(AppConfigParse, AppConfigParseLeavesSeedUnsetByDefault)
{
    // given
    args.push_back("--player=risky");

    // when
    parse();

    // then
    EXPECT_THAT(config.hasSeed, IsFalse());
}

TEST_F(AppConfigParse, AppConfigParseMarksSeedAsSetWhenGiven)
{
    // given
    args.insert(args.end(), {"--player=risky", "--seed=42"});

    // when
    parse();

    // then
    EXPECT_THAT(config.hasSeed, IsTrue());
}

TEST_F(AppConfigParse, AppConfigParseReadsSeed)
{
    // given
    args.insert(args.end(), {"--player=risky", "--seed=18446744073709551615"});

    // when
    parse();

    // then
    EXPECT_THAT(config.seed, Eq(18446744073709551615ull));
}

TEST_F(AppConfigParse, AppConfigParseAcceptsZeroSeed)
{
    // given
    args.insert(args.end(), {"--player=risky", "--seed=0"});

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsTrue());
}

TEST_F(AppConfigParse, AppConfigParseAcceptsNoArguments)
{
    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsTrue());
}

TEST_F(AppConfigParse, AppConfigParseSelectsConsolePlayerByDefault)
{
    // when
    parse();

    // then
    EXPECT_THAT(config.player, Eq(PLAYER_CONSOLE));
}

TEST_F(AppConfigParse, AppConfigParseSelectsConsolePlayerWhenOnlyDecksGiven)
{
    // given
    args.push_back("--decks=2");

    // when
    parse();

    // then
    EXPECT_THAT(config.player, Eq(PLAYER_CONSOLE));
}

TEST_F(AppConfigParse, AppConfigParseSelectsConsolePlayerWhenFlagGiven)
{
    // given
    args.push_back("--player=console");

    // when
    parse();

    // then
    EXPECT_THAT(config.player, Eq(PLAYER_CONSOLE));
}

TEST_F(AppConfigParse, AppConfigParseRejectsConsolePlayerWithThreshold)
{
    // given
    args.push_back("--player=console");
    args.push_back("--player-threshold=15");

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsFalse());
}

TEST_F(AppConfigParse, AppConfigParseKeepsHelpOffByDefault)
{
    // when
    parse();

    // then
    EXPECT_THAT(config.help, IsFalse());
}

TEST_F(AppConfigParse, AppConfigParseSetsHelpWhenHelpFlagGiven)
{
    // given
    args.push_back("--help");

    // when
    parse();

    // then
    EXPECT_THAT(config.help, IsTrue());
}

TEST_F(AppConfigParse, AppConfigParseSetsHelpWhenShortHelpFlagGiven)
{
    // given
    args.push_back("-h");

    // when
    parse();

    // then
    EXPECT_THAT(config.help, IsTrue());
}

TEST_F(AppConfigParse, AppConfigParseAcceptsHelpFlag)
{
    // given
    args.push_back("--help");

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsTrue());
}

TEST_F(AppConfigParse, AppConfigParseAcceptsHelpFlagAlongsideInvalidFlag)
{
    // given
    args.push_back("--bogus");
    args.push_back("--help");

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsTrue());
}
TEST_F(AppConfigParse, AppConfigParseRejectsUnknownPlayer)
{
    // given
    args.push_back("--player=brave");

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsFalse());
}

TEST_F(AppConfigParse, AppConfigParseRejectsUnknownFlag)
{
    // given
    args.insert(args.end(), {"--player=risky", "--verbose"});

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsFalse());
}

TEST_F(AppConfigParse, AppConfigParseRejectsZeroDecks)
{
    // given
    args.insert(args.end(), {"--player=risky", "--decks=0"});

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsFalse());
}

TEST_F(AppConfigParse, AppConfigParseRejectsNonNumericDecks)
{
    // given
    args.insert(args.end(), {"--player=risky", "--decks=many"});

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsFalse());
}

TEST_F(AppConfigParse, AppConfigParseRejectsDecksWithTrailingGarbage)
{
    // given
    args.insert(args.end(), {"--player=risky", "--decks=2x"});

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsFalse());
}

TEST_F(AppConfigParse, AppConfigParseRejectsEmptyDecks)
{
    // given
    args.insert(args.end(), {"--player=risky", "--decks="});

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsFalse());
}

TEST_F(AppConfigParse, AppConfigParseRejectsNonNumericSeed)
{
    // given
    args.insert(args.end(), {"--player=risky", "--seed=abc"});

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsFalse());
}

TEST_F(AppConfigParse, AppConfigParseRejectsNegativeSeed)
{
    // given
    args.insert(args.end(), {"--player=risky", "--seed=-1"});

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsFalse());
}

TEST_F(AppConfigParse, AppConfigParseRejectsNonNumericThreshold)
{
    // given
    args.push_back("--player-threshold=high");

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsFalse());
}

TEST_F(AppConfigParse, AppConfigParseAcceptsPlayerThresholdWithoutPlayer)
{
    // given
    args.push_back("--player-threshold=15");

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsTrue());
}

TEST_F(AppConfigParse, AppConfigParseSelectsCustomPlayerWhenPlayerThresholdGiven)
{
    // given
    args.push_back("--player-threshold=15");

    // when
    parse();

    // then
    EXPECT_THAT(config.player, Eq(PLAYER_CUSTOM));
}

TEST_F(AppConfigParse, AppConfigParseRejectsPlayerTogetherWithPlayerThreshold)
{
    // given
    args.insert(args.end(), {"--player=risky", "--player-threshold=15"});

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsFalse());
}

TEST_F(AppConfigParse, AppConfigParseRejectsPlayerThresholdTogetherWithPlayer)
{
    // given
    args.insert(args.end(), {"--player-threshold=15", "--player=risky"});

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsFalse());
}

TEST_F(AppConfigParse, AppConfigParseRejectsRemovedThresholdFlag)
{
    // given
    args.insert(args.end(), {"--player=risky", "--threshold=15"});

    // when
    const bool accepted = parse();

    // then
    EXPECT_THAT(accepted, IsFalse());
}
} // namespace test
