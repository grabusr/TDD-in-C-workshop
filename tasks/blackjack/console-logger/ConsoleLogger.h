#pragma once

#include <Logger.h>

#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /// Logger writing every message as a separate line to a stream (stdout in the application). The instance must not
    /// be copied or moved after initialization, because `logger.context` points to it. The stream is not owned.
    typedef struct ConsoleLogger
    {
        Logger logger;
        FILE* stream;
    } ConsoleLogger;

    void consoleLoggerInit(ConsoleLogger* self, FILE* stream);

#ifdef __cplusplus
}
#endif
