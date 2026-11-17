#pragma once

#include <Logger.h>

#include <gmock/gmock.h>

#include <string>

namespace test::mocks
{

class LoggerMock
{
public:
    MOCK_METHOD(void, log, (const std::string& message));

    Logger mock{
        .log = &LoggerMock::logMockMethod,
        .context = this,
    };

private:
    static void logMockMethod(void* context, const char* message)
    {
        static_cast<LoggerMock*>(context)->log(message);
    }
};
} // namespace test::mocks
