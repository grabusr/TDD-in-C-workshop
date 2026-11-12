#include "ConsoleLogger.h"

#include <iostream>

void ConsoleLogger::log(const std::string_view message)
{
    std::cout << "[LOG] " << message << '\n';
}
