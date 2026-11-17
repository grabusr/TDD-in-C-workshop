#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum Decision
    {
        DECISION_HIT,
        DECISION_STAND,
    } Decision;

    typedef enum Participant
    {
        PARTICIPANT_PLAYER,
        PARTICIPANT_DEALER,
    } Participant;

#ifdef __cplusplus
}
#endif
