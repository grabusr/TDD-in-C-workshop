#include "Messenger.h"

#include <ILogger.h>
#include <IMessageService.h>

#include <string>
#include <utility>

Messenger::Messenger(std::shared_ptr<ILogger> logger, std::shared_ptr<IMessageService> service)
    : logger{std::move(logger)}
    , service{std::move(service)}
{
    this->logger->log("Messenger Created.");
}

void Messenger::sendMessage(const std::string_view message)
{
    std::string msg{"Sending message: "};
    msg.append(message);
    logger->log(msg);
    service->send(message);
}
