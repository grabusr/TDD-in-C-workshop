#include "ConsoleLogger.h"

static void logToStream(void* const context, const char* const message)
{
    const ConsoleLogger* const self = context;
    if (message == NULL) {
        return;
    }
    fputs(message, self->stream);
    fputc('\n', self->stream);
}

void consoleLoggerInit(ConsoleLogger* const self, FILE* const stream)
{
    self->stream = stream;
    self->logger.log = &logToStream;
    self->logger.context = self;
}
