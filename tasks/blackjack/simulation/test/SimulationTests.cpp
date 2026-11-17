#include <test/mocks/LoggerMock.h>
#include <test/mocks/PlayerStrategyMock.h>
#include <Simulation.h>

#include <gmock/gmock.h>

#include <ostream>

bool operator==(const Card& lhs, const Card& rhs)
{
    return lhs.rank == rhs.rank;
}

void PrintTo(const Card& card, std::ostream* os)
{
    *os << "Card{rank=" << card.rank << "}";
}

namespace test
{
using namespace ::testing;

namespace
{
class Simulation : public Test
{
protected:
    void SetUp() override
    {
        ON_CALL(strategy, decide(_, _)).WillByDefault(Return(DECISION_STAND));
        EXPECT_CALL(logger, log(_)).Times(AnyNumber());
    }

    void TearDown() override
    {
        cardListFree(&deck);
    }

    CardList deck{};
    NiceMock<mocks::PlayerStrategyMock> strategy;
    NiceMock<mocks::LoggerMock> logger;
};

MATCHER_P(IsCard, rank, "")
{
    return arg.rank == rank;
}

MATCHER_P(HandValueIs, value, "")
{
    return handValue(arg) == value;
}

Matcher<Score> IsScore(size_t playerWins, size_t dealerWins, size_t pushes)
{
    return AllOf(Field(&Score::playerWins, playerWins), Field(&Score::dealerWins, dealerWins),
                 Field(&Score::pushes, pushes));
}
} // namespace

TEST_F(Simulation, SimulationRunReturnsEmptyScoreWhenDeckHasNoEnoughCards)
{
    // given
    deck = createCardListFromString("10 10 7");

    // when
    const Score score = simulationRun(&deck, &strategy.mock, &logger.mock);

    // then
    EXPECT_THAT(score, IsScore(0, 0, 0));
}

TEST_F(Simulation, SimulationRunDoesNotAskStrategyWhenDeckHasNoEnoughCards)
{
    // given
    deck = createCardListFromString("10 10 7");

    // expected
    EXPECT_CALL(strategy, decide(_, _)).Times(0);

    // when
    simulationRun(&deck, &strategy.mock, &logger.mock);
}

TEST_F(Simulation, SimulationRunReturnsPlayerWinWhenStrategyStandsAndDealerBusts)
{
    // given
    deck = createCardListFromString("10 10 8 6 K");

    // when
    const Score score = simulationRun(&deck, &strategy.mock, &logger.mock);

    // then
    EXPECT_THAT(score, IsScore(1, 0, 0));
}

TEST_F(Simulation, SimulationRunReturnsPushWhenStrategyStandsOnEqualValues)
{
    // given
    deck = createCardListFromString("10 10 8 8");

    // when
    const Score score = simulationRun(&deck, &strategy.mock, &logger.mock);

    // then
    EXPECT_THAT(score, IsScore(0, 0, 1));
}

TEST_F(Simulation, SimulationRunReturnsDealerWinWhenStrategyHitsAndBusts)
{
    // given
    ON_CALL(strategy, decide(_, _)).WillByDefault(Return(DECISION_HIT));
    deck = createCardListFromString("10 10 5 8 K");

    // when
    const Score score = simulationRun(&deck, &strategy.mock, &logger.mock);

    // then
    EXPECT_THAT(score, IsScore(0, 1, 0));
}

TEST_F(Simulation, SimulationRunPlaysConsecutiveRoundsFromSameDeck)
{
    // given
    deck = createCardListFromString("10 10 8 6 K 10 9 7 K");

    // when
    const Score score = simulationRun(&deck, &strategy.mock, &logger.mock);

    // then
    EXPECT_THAT(score, IsScore(1, 1, 0));
}

TEST_F(Simulation, SimulationRunKeepsScoreWhenLastRoundIsVoid)
{
    // given
    deck = createCardListFromString("10 10 8 6 K 10 9 7");

    // when
    const Score score = simulationRun(&deck, &strategy.mock, &logger.mock);

    // then
    EXPECT_THAT(score, IsScore(1, 0, 0));
}

TEST_F(Simulation, SimulationRunAsksStrategyAgainAfterHit)
{
    // given
    ON_CALL(strategy, decide(_, _)).WillByDefault(Return(DECISION_HIT));
    ON_CALL(strategy, decide(HandValueIs(5), _)).WillByDefault(Return(DECISION_HIT));
    ON_CALL(strategy, decide(HandValueIs(7), _)).WillByDefault(Return(DECISION_HIT));
    ON_CALL(strategy, decide(HandValueIs(9), _)).WillByDefault(Return(DECISION_STAND));
    deck = createCardListFromString("2 10 3 7 2 2");

    // expected
    EXPECT_CALL(strategy, decide(_, _)).Times(3);

    // when
    const Score score = simulationRun(&deck, &strategy.mock, &logger.mock);

    // then
    EXPECT_THAT(score, IsScore(0, 1, 0));
}

TEST_F(Simulation, SimulationRunPassesPlayerHandAndDealerUpCardToStrategy)
{
    // given
    deck = createCardListFromString("10 9 8 K");

    // expected
    EXPECT_CALL(strategy, decide(HandValueIs(18), IsCard(RANK_NINE)));

    // when
    simulationRun(&deck, &strategy.mock, &logger.mock);
}

TEST_F(Simulation, SimulationRunInformsStrategyAboutPlayerAndDealerCardsInOrder)
{
    // given
    deck = createCardListFromString("10 10 8 6 K");

    // expected
    InSequence sequence;
    EXPECT_CALL(strategy, onCardDrawn(PARTICIPANT_PLAYER, IsCard(RANK_TEN)));
    EXPECT_CALL(strategy, onCardDrawn(PARTICIPANT_DEALER, IsCard(RANK_TEN)));
    EXPECT_CALL(strategy, onCardDrawn(PARTICIPANT_PLAYER, IsCard(RANK_EIGHT)));
    EXPECT_CALL(strategy, decide(_, _));
    EXPECT_CALL(strategy, onCardDrawn(PARTICIPANT_DEALER, IsCard(RANK_SIX)));
    EXPECT_CALL(strategy, onCardDrawn(PARTICIPANT_DEALER, IsCard(RANK_KING)));

    // when
    simulationRun(&deck, &strategy.mock, &logger.mock);
}

TEST_F(Simulation, SimulationRunDoesNotInformStrategyAboutDealerHiddenCardWhenPlayerBusts)
{
    // given
    ON_CALL(strategy, decide(_, _)).WillByDefault(Return(DECISION_HIT));
    deck = createCardListFromString("10 10 5 8 K");

    // expected
    EXPECT_CALL(strategy, onCardDrawn(PARTICIPANT_PLAYER, _)).Times(AnyNumber());
    EXPECT_CALL(strategy, onCardDrawn(PARTICIPANT_DEALER, IsCard(RANK_TEN)));
    EXPECT_CALL(strategy, onCardDrawn(PARTICIPANT_DEALER, IsCard(RANK_EIGHT))).Times(0);

    // when
    simulationRun(&deck, &strategy.mock, &logger.mock);
}

TEST_F(Simulation, SimulationRunLogsRoundStartWithHandsAndDealerUpCard)
{
    // given
    deck = createCardListFromString("10 10 8 6 K");

    // expected
    EXPECT_CALL(logger, log("Round 1: player has 10 8 (18), dealer shows 10"));

    // when
    simulationRun(&deck, &strategy.mock, &logger.mock);
}

TEST_F(Simulation, SimulationRunLogsPlayerStand)
{
    // given
    deck = createCardListFromString("10 10 8 6 K");

    // expected
    EXPECT_CALL(logger, log("Player stands with 18"));

    // when
    simulationRun(&deck, &strategy.mock, &logger.mock);
}

TEST_F(Simulation, SimulationRunLogsPlayerHitWithDrawnCardAndHandValue)
{
    // given
    ON_CALL(strategy, decide(_, _)).WillByDefault(Return(DECISION_HIT));
    deck = createCardListFromString("10 10 5 8 K");

    // expected
    EXPECT_CALL(logger, log("Player hits, draws K (hand value 25)"));

    // when
    simulationRun(&deck, &strategy.mock, &logger.mock);
}

TEST_F(Simulation, SimulationRunLogsDealerHandAfterDealerTurn)
{
    // given
    deck = createCardListFromString("10 10 8 6 K");

    // expected
    EXPECT_CALL(logger, log("Dealer plays: 10 6 K (hand value 26)"));

    // when
    simulationRun(&deck, &strategy.mock, &logger.mock);
}

TEST_F(Simulation, SimulationRunLogsPlayerWinOfRound)
{
    // given
    deck = createCardListFromString("10 10 8 6 K");

    // expected
    EXPECT_CALL(logger, log("Round 1: player wins"));

    // when
    simulationRun(&deck, &strategy.mock, &logger.mock);
}

TEST_F(Simulation, SimulationRunLogsDealerWinOfRound)
{
    // given
    ON_CALL(strategy, decide(_, _)).WillByDefault(Return(DECISION_HIT));
    deck = createCardListFromString("10 10 5 8 K");

    // expected
    EXPECT_CALL(logger, log("Round 1: dealer wins"));

    // when
    simulationRun(&deck, &strategy.mock, &logger.mock);
}

TEST_F(Simulation, SimulationRunLogsPushOfRound)
{
    // given
    deck = createCardListFromString("10 10 8 8");

    // expected
    EXPECT_CALL(logger, log("Round 1: push"));

    // when
    simulationRun(&deck, &strategy.mock, &logger.mock);
}

TEST_F(Simulation, SimulationRunNumbersConsecutiveRounds)
{
    // given
    deck = createCardListFromString("10 10 8 6 K 10 9 7 K");

    // expected
    EXPECT_CALL(logger, log("Round 2: player has 10 7 (17), dealer shows 9"));

    // when
    simulationRun(&deck, &strategy.mock, &logger.mock);
}

TEST_F(Simulation, SimulationRunLogsRoundEventsInOrder)
{
    // given
    deck = createCardListFromString("10 10 8 6 K");

    // expected
    InSequence sequence;
    EXPECT_CALL(logger, log("Round 1: player has 10 8 (18), dealer shows 10"));
    EXPECT_CALL(logger, log("Player stands with 18"));
    EXPECT_CALL(logger, log("Dealer plays: 10 6 K (hand value 26)"));
    EXPECT_CALL(logger, log("Round 1: player wins"));
    EXPECT_CALL(logger, log("Deck exhausted, completed rounds: 1"));

    // when
    simulationRun(&deck, &strategy.mock, &logger.mock);
}

TEST_F(Simulation, SimulationRunLogsDeckExhaustionWhenLastRoundIsVoid)
{
    // given
    deck = createCardListFromString("10 10 8 6 K 10 9 7");

    // expected
    EXPECT_CALL(logger, log("Deck exhausted, completed rounds: 1"));

    // when
    simulationRun(&deck, &strategy.mock, &logger.mock);
}

TEST_F(Simulation, SimulationRunDoesNotLogRoundWhenDeckHasNoEnoughCards)
{
    // given
    deck = createCardListFromString("10 10 7");

    // expected
    EXPECT_CALL(logger, log(HasSubstr("Round"))).Times(0);

    // when
    simulationRun(&deck, &strategy.mock, &logger.mock);
}

TEST_F(Simulation, SimulationRunReturnsScoreWhenLoggerIsNull)
{
    // given
    deck = createCardListFromString("10 10 8 6 K");

    // when
    const Score score = simulationRun(&deck, &strategy.mock, nullptr);

    // then
    EXPECT_THAT(score, IsScore(1, 0, 0));
}
} // namespace test
