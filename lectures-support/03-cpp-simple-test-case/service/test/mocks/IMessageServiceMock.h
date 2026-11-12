#pragma once

#include <IMessageService.h>

#include <gmock/gmock.h>

namespace test
{
class IMessageServiceMock : public IMessageService
{
public:
    MOCK_METHOD(void, send, (std::string_view message), (override));
};
}
