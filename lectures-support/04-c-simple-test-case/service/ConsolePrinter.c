#include "ConsolePrinter.h"

#include <stdio.h>

static void consolePrinterSend(void* context, const char* message)
{
    (void)context;
    printf("%s\n", message);
}

MessageService consolePrinterCreate(void)
{
    const MessageService service = {
        .send = consolePrinterSend,
        .context = NULL,
    };
    return service;
}
