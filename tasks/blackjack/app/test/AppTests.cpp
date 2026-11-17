#include <test/mocks/LoggerMock.h>
#include <App.h>

#include <gmock/gmock.h>

#include <cstdio>

namespace test
{
using namespace ::testing;

namespace
{
class App : public Test
{
protected:
    void SetUp() override
    {
        input = std::tmpfile();
        config = AppConfig{.player = PLAYER_CONSOLE, .threshold = 0, .deckCount = 1};
        EXPECT_CALL(logger, log(_)).Times(AnyNumber());
    }

    void TearDown() override
    {
        std::fclose(input);
    }

    FILE* input{};
    NiceMock<mocks::LoggerMock> logger;
    AppConfig config{};
    Score score{};
};
} // namespace

TEST_F(App, AppPrintUsageMentionsProgramName)
{
    // expected
    EXPECT_CALL(logger, log(HasSubstr("Usage: blackjack"))).Times(AtLeast(1));

    // when
    appPrintUsage("blackjack", &logger.mock);
}

TEST_F(App, AppPrintUsageDescribesPlayerFlag)
{
    // expected
    EXPECT_CALL(logger, log(HasSubstr("--player=console|cautious|risky"))).Times(AtLeast(1));

    // when
    appPrintUsage("blackjack", &logger.mock);
}

TEST_F(App, AppPrintUsageDescribesPlayerThresholdFlag)
{
    // expected
    EXPECT_CALL(logger, log(HasSubstr("--player-threshold=N"))).Times(AtLeast(1));

    // when
    appPrintUsage("blackjack", &logger.mock);
}

TEST_F(App, AppPrintUsageDescribesDecksFlag)
{
    // expected
    EXPECT_CALL(logger, log(HasSubstr("--decks=N"))).Times(AtLeast(1));

    // when
    appPrintUsage("blackjack", &logger.mock);
}

TEST_F(App, AppPrintUsageDescribesSeedFlag)
{
    // expected
    EXPECT_CALL(logger, log(HasSubstr("--seed=N"))).Times(AtLeast(1));

    // when
    appPrintUsage("blackjack", &logger.mock);
}

TEST_F(App, AppPrintUsageDescribesHelpFlag)
{
    // expected
    EXPECT_CALL(logger, log(HasSubstr("--help"))).Times(AtLeast(1));

    // when
    appPrintUsage("blackjack", &logger.mock);
}

TEST_F(App, AppPrintUsageDescribesConsoleControls)
{
    // expected
    EXPECT_CALL(logger, log(HasSubstr("h = hit, s = stand"))).Times(AtLeast(1));

    // when
    appPrintUsage("blackjack", &logger.mock);
}

TEST_F(App, AppPlayAsksHumanWhenPlayerIsConsole)
{
    // expected
    EXPECT_CALL(logger, log(HasSubstr("[h]it or [s]tand"))).Times(AtLeast(1));

    // when
    appPlay(&config, 1, input, &logger.mock, &score);
}

TEST_F(App, AppPlayReadsDecisionsFromInputWhenPlayerIsConsole)
{
    // given
    std::fputs("h\nh\nh\nh\nh\nh\nh\nh\nh\nh\nh\nh\nh\nh\nh\nh\nh\nh\nh\nh\n", input);
    std::rewind(input);
    config.deckCount = 1;

    // when
    appPlay(&config, 1, input, &logger.mock, &score);

    // then
    EXPECT_THAT(score.dealerWins, Gt(0u));
}

TEST_F(App, AppPlayDoesNotAskHumanWhenPlayerIsRisky)
{
    // given
    config.player = PLAYER_RISKY;
    config.threshold = 17;
    // expected
    EXPECT_CALL(logger, log(HasSubstr("[h]it or [s]tand"))).Times(0);

    // when
    appPlay(&config, 1, input, &logger.mock, &score);
}

TEST_F(App, AppPlayReturnsTrueWhenGamePlayed)
{
    // when
    const bool played = appPlay(&config, 1, input, &logger.mock, &score);

    // then
    EXPECT_THAT(played, IsTrue());
}
} // namespace test
