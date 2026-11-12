#include "Messenger.h"

#include <ILogger.h>
#include <IMessageService.h>

#include <string>
#include <utility>

Messenger::Messenger(std::shared_ptr<ILogger> logger, std::shared_ptr<IMessageService> service)
    : logger{std::move(logger)}
    , service{std::move(service)}
{
    this->logger->log("Creating Messenger");
}

void Messenger::sendMessage(const std::string_view message)
{
    logger->log("Sending message: " + std::string{message});
    service->send(message);
}
