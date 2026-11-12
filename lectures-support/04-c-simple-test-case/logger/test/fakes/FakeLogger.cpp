#include "FakeLogger.h"

namespace test::fakes::logger
{
namespace
{
std::vector<std::string> loggedMessages;
} // namespace

const std::vector<std::string>& messages()
{
    return loggedMessages;
}

void reset()
{
    loggedMessages.clear();
}
} // namespace test::fakes::logger

extern "C" void logMessage(const char* message)
{
    test::fakes::logger::loggedMessages.emplace_back(message);
}
