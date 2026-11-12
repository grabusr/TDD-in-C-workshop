#pragma once

#include <ILogger.h>

#include <gmock/gmock.h>

namespace test
{
class ILoggerMock : public ILogger
{
public:
    ~ILoggerMock() override = default;
    MOCK_METHOD(void, log, (std::string_view message), (override));
};
}
