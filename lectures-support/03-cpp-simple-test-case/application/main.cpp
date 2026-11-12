#include "ConsoleLogger.h"

#include <ConsolePrinter.h>
#include <Messenger.h>

#include <cstdlib>
#include <memory>

int main()
{
    Messenger messenger{std::make_shared<ConsoleLogger>(), std::make_shared<ConsolePrinter>()};

    messenger.sendMessage("Hello, World!");

    return EXIT_SUCCESS;
}
