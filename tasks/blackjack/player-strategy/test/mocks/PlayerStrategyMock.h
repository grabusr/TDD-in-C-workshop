#pragma once

#include <PlayerStrategy.h>

#include <gmock/gmock.h>

namespace test::mocks
{

class PlayerStrategyMock
{
public:
    MOCK_METHOD(Decision, decide, (const Hand* playerHand, Card dealerUpCard));
    MOCK_METHOD(void, onCardDrawn, (Participant who, Card card));

    PlayerStrategy mock{
        .decide = &PlayerStrategyMock::decideMockMethod,
        .onCardDrawn = &PlayerStrategyMock::onCardDrawnMockMethod,
        .context = this,
    };

private:
    static Decision decideMockMethod(void* context, const Hand* playerHand, Card dealerUpCard)
    {
        return static_cast<PlayerStrategyMock*>(context)->decide(playerHand, dealerUpCard);
    }

    static void onCardDrawnMockMethod(void* context, Participant who, Card card)
    {
        static_cast<PlayerStrategyMock*>(context)->onCardDrawn(who, card);
    }
};
} // namespace test::mocks
