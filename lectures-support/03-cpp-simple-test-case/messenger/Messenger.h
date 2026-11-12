#pragma once

#include <memory>
#include <string_view>

class ILogger;
class IMessageService;

class Messenger
{
public:
    Messenger(std::shared_ptr<ILogger> logger, std::shared_ptr<IMessageService> service);

    void sendMessage(std::string_view message);

private:
    std::shared_ptr<ILogger> logger;
    std::shared_ptr<IMessageService> service;
};
