#include "ConsolePrinter.h"

#include <iostream>

void ConsolePrinter::send(std::string_view message)
{
    std::cout << message << '\n';
}
