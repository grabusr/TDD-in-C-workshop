#pragma once

#include <string_view>

class IMessageService
{
public:
    virtual ~IMessageService() = default;

    virtual void send(std::string_view message) = 0;
};
