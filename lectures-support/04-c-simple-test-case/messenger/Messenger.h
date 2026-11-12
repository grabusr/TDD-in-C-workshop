#pragma once

#include <MessageService.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Messenger
{
    MessageService service;
} Messenger;

Messenger messengerCreate(MessageService service);
void messengerSendMessage(Messenger* messenger, const char* message);

#ifdef __cplusplus
}
#endif
