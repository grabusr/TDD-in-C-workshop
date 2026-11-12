#include <stdlib.h>

#include <ConsolePrinter.h>
#include <Messenger.h>

int main(void)
{
    Messenger messenger = messengerCreate(consolePrinterCreate());

    messengerSendMessage(&messenger, "Hello, World!");

    return EXIT_SUCCESS;
}
