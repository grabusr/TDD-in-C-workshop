#include "ThresholdPlayerStrategy.h"

static Decision decide(void* context, const Hand* playerHand, Card dealerUpCard)
{
    (void)dealerUpCard;
    const ThresholdPlayerStrategy* self = context;
    return handValue(playerHand) >= self->standThreshold ? DECISION_STAND : DECISION_HIT;
}

static void onCardDrawn(void* context, Participant who, Card card)
{
    (void)context;
    (void)who;
    (void)card;
}

void thresholdPlayerStrategyInit(ThresholdPlayerStrategy* self, int standThreshold)
{
    self->standThreshold = standThreshold;
    self->strategy.decide = decide;
    self->strategy.onCardDrawn = onCardDrawn;
    self->strategy.context = self;
}
