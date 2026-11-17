#include <ThresholdPlayerStrategy.h>

#include <gmock/gmock.h>

namespace test
{
using namespace ::testing;

namespace
{
class ThresholdPlayerStrategy : public Test
{
protected:
    void SetUp() override
    {
        thresholdPlayerStrategyInit(&sut, 12);
    }

    ::ThresholdPlayerStrategy sut{};
    Card dealerUpCard{RANK_TEN};
};

TEST_F(ThresholdPlayerStrategy, DecideReturnsHitWhenHandValueBelowThreshold)
{
    // given
    const Hand hand{{{RANK_TWO}, {RANK_NINE}}, 2};

    // when
    const Decision decision = sut.strategy.decide(sut.strategy.context, &hand, dealerUpCard);

    // then
    EXPECT_THAT(decision, Eq(DECISION_HIT));
}

TEST_F(ThresholdPlayerStrategy, DecideReturnsStandWhenHandValueEqualsThreshold)
{
    // given
    const Hand hand{{{RANK_THREE}, {RANK_NINE}}, 2};

    // when
    const Decision decision = sut.strategy.decide(sut.strategy.context, &hand, dealerUpCard);

    // then
    EXPECT_THAT(decision, Eq(DECISION_STAND));
}

TEST_F(ThresholdPlayerStrategy, DecideReturnsStandWhenHandValueAboveThreshold)
{
    // given
    const Hand hand{{{RANK_KING}, {RANK_QUEEN}}, 2};

    // when
    const Decision decision = sut.strategy.decide(sut.strategy.context, &hand, dealerUpCard);

    // then
    EXPECT_THAT(decision, Eq(DECISION_STAND));
}

TEST_F(ThresholdPlayerStrategy, DecideReturnsStandWhenSoftHandValueReachesThreshold)
{
    // given
    const Hand hand{{{RANK_ACE}, {RANK_ACE}}, 2};

    // when
    const Decision decision = sut.strategy.decide(sut.strategy.context, &hand, dealerUpCard);

    // then
    EXPECT_THAT(decision, Eq(DECISION_STAND));
}

TEST_F(ThresholdPlayerStrategy, DecideReturnsSameDecisionForAnyDealerUpCard)
{
    // given
    const Hand hand{{{RANK_TWO}, {RANK_NINE}}, 2};

    // when
    const Decision decision = sut.strategy.decide(sut.strategy.context, &hand, Card{RANK_ACE});

    // then
    EXPECT_THAT(decision, Eq(DECISION_HIT));
}

TEST_F(ThresholdPlayerStrategy, DecideUsesConfiguredThreshold)
{
    // given
    thresholdPlayerStrategyInit(&sut, 17);
    const Hand hand{{{RANK_TEN}, {RANK_SIX}}, 2};

    // when
    const Decision decision = sut.strategy.decide(sut.strategy.context, &hand, dealerUpCard);

    // then
    EXPECT_THAT(decision, Eq(DECISION_HIT));
}

TEST_F(ThresholdPlayerStrategy, InitSetsOnCardDrawnCallback)
{
    // then
    EXPECT_THAT(sut.strategy.onCardDrawn, NotNull());
}

TEST_F(ThresholdPlayerStrategy, InitSetsContextToItself)
{
    // then
    EXPECT_THAT(sut.strategy.context, Eq(&sut));
}

} // namespace
} // namespace test
