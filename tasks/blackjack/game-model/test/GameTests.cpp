#include <Game.h>

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
class BlackjackGame : public Test
{
protected:
    void TearDown() override
    {
        cardListFree(&deck);
    }

    void startGameWith(const char* cards)
    {
        deck = createCardListFromString(cards);
        gameInit(&game, &deck);
    }

    CardList deck{};
    Game game{};
};

MATCHER_P(IsCard, rank, "")
{
    return arg.rank == rank;
}
} // namespace

TEST_F(BlackjackGame, GameStartRoundAwaitsDecisionWhenDeckHasFourCards)
{
    // given
    startGameWith("10 10 8 6");

    // when
    const RoundState state = gameStartRound(&game);

    // then
    EXPECT_THAT(state, Eq(ROUND_AWAITING_DECISION));
}

TEST_F(BlackjackGame, GameStartRoundDealsPlayerFirstAndThirdCard)
{
    // given
    startGameWith("10 9 8 6");

    // when
    gameStartRound(&game);

    // then
    EXPECT_THAT(handValue(gamePlayerHand(&game)), Eq(18));
}

TEST_F(BlackjackGame, GameDealerUpCardReturnsDealerFirstCard)
{
    // given
    startGameWith("10 9 8 6");
    gameStartRound(&game);

    // when
    const Card upCard = gameDealerUpCard(&game);

    // then
    EXPECT_THAT(upCard, IsCard(RANK_NINE));
}

TEST_F(BlackjackGame, GameDealerHandContainsCardsDrawnByDealerAfterStand)
{
    // given
    startGameWith("10 2 8 3 2 2 K 5");
    gameStartRound(&game);

    // when
    gameApplyDecision(&game, DECISION_STAND);

    // then
    EXPECT_THAT(handValue(gameDealerHand(&game)), Eq(19));
}

TEST_F(BlackjackGame, GameStartRoundReturnsVoidWhenDeckHasThreeCards)
{
    // given
    startGameWith("10 10 7");

    // when
    const RoundState state = gameStartRound(&game);

    // then
    EXPECT_THAT(state, Eq(ROUND_VOID));
}

TEST_F(BlackjackGame, GameStartRoundEndsGameWhenDeckHasThreeCards)
{
    // given
    startGameWith("10 10 7");

    // when
    gameStartRound(&game);

    // then
    EXPECT_THAT(gameIsOver(&game), IsTrue());
}

TEST_F(BlackjackGame, GameStartRoundKeepsScoreWhenDeckHasThreeCards)
{
    // given
    startGameWith("10 10 7");

    // when
    gameStartRound(&game);

    // then
    EXPECT_THAT(gameScore(&game),
                AllOf(Field(&Score::playerWins, 0u), Field(&Score::dealerWins, 0u), Field(&Score::pushes, 0u)));
}

TEST_F(BlackjackGame, GameApplyDecisionReturnsPlayerWonWhenDealerBustsAfterStand)
{
    // given
    startGameWith("10 10 8 6 K");
    gameStartRound(&game);

    // when
    const RoundState state = gameApplyDecision(&game, DECISION_STAND);

    // then
    EXPECT_THAT(state, Eq(ROUND_PLAYER_WON));
}

TEST_F(BlackjackGame, GameApplyDecisionCountsPlayerWinInScore)
{
    // given
    startGameWith("10 10 8 6 K");
    gameStartRound(&game);

    // when
    gameApplyDecision(&game, DECISION_STAND);

    // then
    EXPECT_THAT(gameScore(&game).playerWins, Eq(1u));
}

TEST_F(BlackjackGame, GameApplyDecisionReturnsDealerWonWhenPlayerBustsAfterHit)
{
    // given
    startGameWith("10 10 5 8 K");
    gameStartRound(&game);

    // when
    const RoundState state = gameApplyDecision(&game, DECISION_HIT);

    // then
    EXPECT_THAT(state, Eq(ROUND_DEALER_WON));
}

TEST_F(BlackjackGame, GameApplyDecisionCountsDealerWinInScoreWhenPlayerBusts)
{
    // given
    startGameWith("10 10 5 8 K");
    gameStartRound(&game);

    // when
    gameApplyDecision(&game, DECISION_HIT);

    // then
    EXPECT_THAT(gameScore(&game).dealerWins, Eq(1u));
}

TEST_F(BlackjackGame, GameApplyDecisionAwaitsDecisionWhenPlayerHitsWithoutBusting)
{
    // given
    startGameWith("2 10 3 7 2 2");
    gameStartRound(&game);

    // when
    const RoundState state = gameApplyDecision(&game, DECISION_HIT);

    // then
    EXPECT_THAT(state, Eq(ROUND_AWAITING_DECISION));
}

TEST_F(BlackjackGame, GameApplyDecisionAddsDrawnCardToPlayerHandWhenHit)
{
    // given
    startGameWith("2 10 3 7 2 2");
    gameStartRound(&game);

    // when
    gameApplyDecision(&game, DECISION_HIT);

    // then
    EXPECT_THAT(handValue(gamePlayerHand(&game)), Eq(7));
}

TEST_F(BlackjackGame, GameApplyDecisionReturnsDealerWonWhenDealerHasHigherValue)
{
    // given
    startGameWith("2 10 3 7 2 2");
    gameStartRound(&game);
    gameApplyDecision(&game, DECISION_HIT);
    gameApplyDecision(&game, DECISION_HIT);

    // when
    const RoundState state = gameApplyDecision(&game, DECISION_STAND);

    // then
    EXPECT_THAT(state, Eq(ROUND_DEALER_WON));
}

TEST_F(BlackjackGame, GameApplyDecisionReturnsDealerWonWhenPlayerHasLowerValue)
{
    // given
    startGameWith("10 9 7 K");
    gameStartRound(&game);

    // when
    const RoundState state = gameApplyDecision(&game, DECISION_STAND);

    // then
    EXPECT_THAT(state, Eq(ROUND_DEALER_WON));
}

TEST_F(BlackjackGame, GameApplyDecisionReturnsPushWhenValuesAreEqual)
{
    // given
    startGameWith("10 10 8 8");
    gameStartRound(&game);

    // when
    const RoundState state = gameApplyDecision(&game, DECISION_STAND);

    // then
    EXPECT_THAT(state, Eq(ROUND_PUSH));
}

TEST_F(BlackjackGame, GameApplyDecisionCountsPushInScore)
{
    // given
    startGameWith("10 10 8 8");
    gameStartRound(&game);

    // when
    gameApplyDecision(&game, DECISION_STAND);

    // then
    EXPECT_THAT(gameScore(&game).pushes, Eq(1u));
}

TEST_F(BlackjackGame, GameApplyDecisionMakesDealerDrawUntilSeventeenOrMore)
{
    // given
    startGameWith("10 2 8 3 2 2 K 5");
    gameStartRound(&game);

    // when
    const RoundState state = gameApplyDecision(&game, DECISION_STAND);

    // then
    EXPECT_THAT(state, Eq(ROUND_DEALER_WON));
}

TEST_F(BlackjackGame, GameStartRoundReturnsVoidWhenOneCardRemainsAfterRound)
{
    // given
    startGameWith("10 2 8 3 2 2 K 5");
    gameStartRound(&game);
    gameApplyDecision(&game, DECISION_STAND);

    // when
    const RoundState state = gameStartRound(&game);

    // then
    EXPECT_THAT(state, Eq(ROUND_VOID));
}

TEST_F(BlackjackGame, GameApplyDecisionReturnsPlayerWonWhenDealerStandsOnSoftSeventeen)
{
    // given
    startGameWith("10 A 9 6 5");
    gameStartRound(&game);

    // when
    const RoundState state = gameApplyDecision(&game, DECISION_STAND);

    // then
    EXPECT_THAT(state, Eq(ROUND_PLAYER_WON));
}

TEST_F(BlackjackGame, GameApplyDecisionReturnsVoidWhenDeckRunsOutDuringPlayerTurn)
{
    // given
    startGameWith("2 10 3 7");
    gameStartRound(&game);

    // when
    const RoundState state = gameApplyDecision(&game, DECISION_HIT);

    // then
    EXPECT_THAT(state, Eq(ROUND_VOID));
}

TEST_F(BlackjackGame, GameApplyDecisionReturnsVoidWhenDeckRunsOutDuringDealerTurn)
{
    // given
    startGameWith("10 10 8 6");
    gameStartRound(&game);

    // when
    const RoundState state = gameApplyDecision(&game, DECISION_STAND);

    // then
    EXPECT_THAT(state, Eq(ROUND_VOID));
}

TEST_F(BlackjackGame, GameApplyDecisionKeepsScoreWhenDeckRunsOutDuringDealerTurn)
{
    // given
    startGameWith("10 10 8 6");
    gameStartRound(&game);

    // when
    gameApplyDecision(&game, DECISION_STAND);

    // then
    EXPECT_THAT(gameScore(&game),
                AllOf(Field(&Score::playerWins, 0u), Field(&Score::dealerWins, 0u), Field(&Score::pushes, 0u)));
}

TEST_F(BlackjackGame, GameApplyDecisionEndsGameWhenDeckRunsOutDuringDealerTurn)
{
    // given
    startGameWith("10 10 8 6");
    gameStartRound(&game);

    // when
    gameApplyDecision(&game, DECISION_STAND);

    // then
    EXPECT_THAT(gameIsOver(&game), IsTrue());
}

TEST_F(BlackjackGame, GameStartRoundReturnsVoidWhenGameIsOver)
{
    // given
    startGameWith("10 10 8 6 K 10 9 7 K");
    gameStartRound(&game);
    gameApplyDecision(&game, DECISION_STAND);
    gameStartRound(&game);
    gameApplyDecision(&game, DECISION_STAND);

    // when
    const RoundState state = gameStartRound(&game);

    // then
    EXPECT_THAT(state, Eq(ROUND_VOID));
}

TEST_F(BlackjackGame, GameScoreCountsWinsOfBothSidesAfterTwoRounds)
{
    // given
    startGameWith("10 10 8 6 K 10 9 7 K");
    gameStartRound(&game);
    gameApplyDecision(&game, DECISION_STAND);
    gameStartRound(&game);
    gameApplyDecision(&game, DECISION_STAND);

    // when
    const Score score = gameScore(&game);

    // then
    EXPECT_THAT(score, AllOf(Field(&Score::playerWins, 1u), Field(&Score::dealerWins, 1u), Field(&Score::pushes, 0u)));
}

TEST_F(BlackjackGame, GameStartRoundClearsHandsFromPreviousRound)
{
    // given
    startGameWith("10 10 8 6 K 10 9 7 K");
    gameStartRound(&game);
    gameApplyDecision(&game, DECISION_STAND);

    // when
    gameStartRound(&game);

    // then
    EXPECT_THAT(handValue(gamePlayerHand(&game)), Eq(17));
}

TEST_F(BlackjackGame, GameInitResetsScore)
{
    // given
    startGameWith("10 10 8 6 K");
    gameStartRound(&game);
    gameApplyDecision(&game, DECISION_STAND);

    // when
    gameInit(&game, &deck);

    // then
    EXPECT_THAT(gameScore(&game).playerWins, Eq(0u));
}

} // namespace test
