#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    /// Sink for human readable messages describing what happens in the game. One call is one message (one line);
    /// the message has no trailing newline and is valid only for the duration of the call.
    typedef struct Logger
    {
        void (*log)(void* context, const char* message);
        void* context;
    } Logger;

#ifdef __cplusplus
}
#endif
