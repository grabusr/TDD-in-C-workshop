#pragma once

#include "ILogger.h"

class ConsoleLogger : public ILogger
{
public:
    void log(std::string_view message) override;
};
