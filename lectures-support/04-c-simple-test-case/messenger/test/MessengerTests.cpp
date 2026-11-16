extern "C"
{
#include <Messenger.h>
}

#include <test/fakes/FakeLogger.h>
#include <test/mocks/MessageServiceMock.h>

#include <gmock/gmock.h>

namespace test
{
using namespace ::testing;

namespace
{
class MessengerTest : public Test
{
protected:
    void SetUp() override
    {
        fakes::logger::reset();
    }

    NiceMock<mocks::MessageServiceMock> serviceMock;
};
} // namespace

TEST_F(MessengerTest, CreateLogsCreatingMessenger)
{
    // when
    messengerCreate(serviceMock.mock);

    // then
    EXPECT_THAT(fakes::logger::messages(), ElementsAre(StrEq("Creating Messenger")));
}

TEST_F(MessengerTest, SendMessageLogsSendingMessage)
{
    // given
    auto messenger = messengerCreate(serviceMock.mock);

    // when
    messengerSendMessage(&messenger, "Hello");

    // then
    EXPECT_THAT(fakes::logger::messages(), ElementsAre(_, StrEq("Sending message: Hello")));
}

TEST_F(MessengerTest, SendMessageForwardsMessageToService)
{
    // given
    auto messenger = messengerCreate(serviceMock.mock);

    // expected
    EXPECT_CALL(serviceMock, send(StrEq("Hello")));

    // when
    messengerSendMessage(&messenger, "Hello");
}

TEST_F(MessengerTest, CreateDoesNotSendAnyMessage)
{
    // expected
    EXPECT_CALL(serviceMock, send(_)).Times(0);

    // when
    messengerCreate(serviceMock.mock);
}

} // namespace test
