#include <Messenger.h>

#include <test/mocks/ILoggerMock.h>
#include <test/mocks/IMessageServiceMock.h>

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
        logger = std::make_shared<NiceMock<ILoggerMock>>();
        service = std::make_shared<IMessageServiceMock>();
    }

    std::shared_ptr<ILoggerMock> logger;
    std::shared_ptr<IMessageServiceMock> service;
};
} // namespace

TEST_F(MessengerTest, SendingTest)
{
    // given
    constexpr auto message = "Hello";

    // expected
    EXPECT_CALL(*logger, log(_)).Times(2);
    EXPECT_CALL(*service, send(StrEq(message)));

    // when
    const auto messenger = std::make_unique<Messenger>(logger, service);
    messenger->sendMessage(message);
}

TEST_F(MessengerTest, SendMessageToService)
{
    // given
    constexpr auto message = "Hello";
    auto messenger = Messenger(logger, service);

    // expected
    EXPECT_CALL(*service, send(StrEq(message)));

    // when
    messenger.sendMessage(message);
}

TEST_F(MessengerTest, SendMessageLogsMessageBeforeSending)
{
    // given
    constexpr auto message = "Hello";
    auto messenger = Messenger(logger, service);

    // expected
    InSequence s;
    EXPECT_CALL(*logger, log(StrEq("Sending message: Hello")));
    EXPECT_CALL(*service, send(_));

    // when
    messenger.sendMessage(message);
}

TEST_F(MessengerTest, MessengerLogsCreation)
{
    // expected
    EXPECT_CALL(*logger, log(StrEq("Messenger Created.")));

    // when
    Messenger(logger, service);
}

} // namespace test
