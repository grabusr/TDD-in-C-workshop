#include "Messenger.h"

#include <stdio.h>

#include <Logger.h>

enum
{
    logBufferSize = 256
};

Messenger messengerCreate(const MessageService service)
{
    logMessage("Creating Messenger");

    const Messenger messenger = {service};
    return messenger;
}

void messengerSendMessage(Messenger* const messenger, const char* const message)
{
    char logLine[logBufferSize];
    snprintf(logLine, sizeof(logLine), "Sending message: %s", message);
    logMessage(logLine);

    messenger->service.send(messenger->service.context, message);
}
