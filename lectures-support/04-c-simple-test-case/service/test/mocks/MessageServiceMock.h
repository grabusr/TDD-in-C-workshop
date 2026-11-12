#pragma once

#include <MessageService.h>

#include <gmock/gmock.h>

namespace test::mocks
{

class MessageServiceMock
{
public:
    MOCK_METHOD(void, send, (const char* message));

    MessageService mock{
        .send = &MessageServiceMock::sendMockMethod,
        .context = this,
    };

private:
    static void sendMockMethod(void* context, const char* message)
    {
        static_cast<MessageServiceMock*>(context)->send(message);
    }
};
} // namespace test::mocks
