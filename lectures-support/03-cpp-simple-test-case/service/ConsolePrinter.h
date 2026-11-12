#pragma once

#include "IMessageService.h"

class ConsolePrinter : public IMessageService
{
public:
    void send(std::string_view message) override;
};
