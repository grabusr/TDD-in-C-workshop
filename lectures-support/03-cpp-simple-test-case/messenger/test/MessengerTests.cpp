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
        logger = std::make_shared<ILoggerMock>();
        // ON_CALL(*logger, log(_));

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

} // namespace test
