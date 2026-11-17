#include <ConsoleLogger.h>

#include <gmock/gmock.h>

#include <cstdio>
#include <string>

namespace test
{
using namespace ::testing;

namespace
{
class ConsoleLogger : public Test
{
protected:
    void SetUp() override
    {
        file = std::tmpfile();
        consoleLoggerInit(&consoleLogger, file);
    }

    void TearDown() override
    {
        std::fclose(file);
    }

    std::string written()
    {
        std::fflush(file);
        std::rewind(file);
        char buffer[256]{};
        std::fread(buffer, 1, sizeof(buffer) - 1, file);
        return buffer;
    }

    ::ConsoleLogger consoleLogger{};
    std::FILE* file{nullptr};
};
} // namespace

TEST_F(ConsoleLogger, LogWritesMessageFollowedByNewline)
{
    // when
    consoleLogger.logger.log(consoleLogger.logger.context, "Round 1 started");

    // then
    EXPECT_THAT(written(), StrEq("Round 1 started\n"));
}

TEST_F(ConsoleLogger, LogWritesConsecutiveMessagesAsSeparateLines)
{
    // when
    consoleLogger.logger.log(consoleLogger.logger.context, "first");
    consoleLogger.logger.log(consoleLogger.logger.context, "second");

    // then
    EXPECT_THAT(written(), StrEq("first\nsecond\n"));
}

TEST_F(ConsoleLogger, LogWritesNothingWhenMessageIsNull)
{
    // when
    consoleLogger.logger.log(consoleLogger.logger.context, nullptr);

    // then
    EXPECT_THAT(written(), IsEmpty());
}
} // namespace test
