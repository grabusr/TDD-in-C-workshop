#include <test/mocks/LoggerMock.h>
#include <ConsolePlayerStrategy.h>

#include <gmock/gmock.h>

#include <cstdio>
#include <string>

namespace test
{
using namespace ::testing;

namespace
{
class ConsolePlayerStrategy : public Test
{
protected:
    void SetUp() override
    {
        input = std::tmpfile();
        hand = Hand{{{RANK_TEN}, {RANK_FIVE}}, 2};
        dealerUpCard = Card{RANK_KING};
    }

    void TearDown() override
    {
        std::fclose(input);
    }

    void typeIn(const std::string& text)
    {
        std::fputs(text.c_str(), input);
        std::rewind(input);
        ::consolePlayerStrategyInit(&sut, input, &logger.mock);
    }

    Decision decide()
    {
        return sut.strategy.decide(sut.strategy.context, &hand, dealerUpCard);
    }

    FILE* input{};
    NiceMock<mocks::LoggerMock> logger;
    ::ConsolePlayerStrategy sut{};
    Hand hand{};
    Card dealerUpCard{};
};
} // namespace

TEST_F(ConsolePlayerStrategy, InitSetsContextToItself)
{
    // when
    typeIn("");

    // then
    EXPECT_THAT(sut.strategy.context, Eq(&sut));
}

TEST_F(ConsolePlayerStrategy, DecideReturnsHitWhenInputIsH)
{
    // given
    typeIn("h\n");

    // when
    const Decision decision = decide();

    // then
    EXPECT_THAT(decision, Eq(DECISION_HIT));
}

TEST_F(ConsolePlayerStrategy, DecideReturnsStandWhenInputIsS)
{
    // given
    typeIn("s\n");

    // when
    const Decision decision = decide();

    // then
    EXPECT_THAT(decision, Eq(DECISION_STAND));
}

TEST_F(ConsolePlayerStrategy, DecideReturnsHitWhenInputIsUpperCaseH)
{
    // given
    typeIn("H\n");

    // when
    const Decision decision = decide();

    // then
    EXPECT_THAT(decision, Eq(DECISION_HIT));
}

TEST_F(ConsolePlayerStrategy, DecideReturnsStandWhenInputIsUpperCaseS)
{
    // given
    typeIn("S\n");

    // when
    const Decision decision = decide();

    // then
    EXPECT_THAT(decision, Eq(DECISION_STAND));
}

TEST_F(ConsolePlayerStrategy, DecideIgnoresSurroundingWhitespace)
{
    // given
    typeIn("  h  \n");

    // when
    const Decision decision = decide();

    // then
    EXPECT_THAT(decision, Eq(DECISION_HIT));
}

TEST_F(ConsolePlayerStrategy, DecideAsksAgainWhenInputIsInvalid)
{
    // given
    typeIn("x\nhit\nh\n");

    // when
    const Decision decision = decide();

    // then
    EXPECT_THAT(decision, Eq(DECISION_HIT));
}

TEST_F(ConsolePlayerStrategy, DecideReportsInvalidInput)
{
    // given
    typeIn("x\ns\n");
    // expected
    EXPECT_CALL(logger, log(_)).Times(AnyNumber());
    EXPECT_CALL(logger, log(HasSubstr("Unknown answer"))).Times(1);

    // when
    decide();
}

TEST_F(ConsolePlayerStrategy, DecideReturnsStandWhenInputEnds)
{
    // given
    typeIn("");

    // when
    const Decision decision = decide();

    // then
    EXPECT_THAT(decision, Eq(DECISION_STAND));
}

TEST_F(ConsolePlayerStrategy, DecideReturnsStandWhenInputEndsAfterInvalidAnswer)
{
    // given
    typeIn("x\n");

    // when
    const Decision decision = decide();

    // then
    EXPECT_THAT(decision, Eq(DECISION_STAND));
}

TEST_F(ConsolePlayerStrategy, DecidePromptsWithHandAndValue)
{
    // given
    typeIn("s\n");
    // expected
    EXPECT_CALL(logger, log(HasSubstr("10 5 (15)"))).Times(1);

    // when
    decide();
}

TEST_F(ConsolePlayerStrategy, DecidePromptsWithDealerUpCard)
{
    // given
    typeIn("s\n");
    // expected
    EXPECT_CALL(logger, log(HasSubstr("dealer shows K"))).Times(1);

    // when
    decide();
}

TEST_F(ConsolePlayerStrategy, DecidePromptsWithAvailableAnswers)
{
    // given
    typeIn("s\n");
    // expected
    EXPECT_CALL(logger, log(HasSubstr("[h]it or [s]tand"))).Times(1);

    // when
    decide();
}
} // namespace test
