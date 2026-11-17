#include "HeapFake.h"

#include <App.h>
#include <test/mocks/LoggerMock.h>

#include <gmock/gmock.h>

#include <cstdio>

namespace test
{
using namespace ::testing;

namespace
{
class AppMemory : public Test
{
protected:
    void SetUp() override
    {
        input = std::tmpfile();
        std::fputs("h\ns\nh\ns\nh\ns\nh\ns\nh\ns\nh\ns\nh\ns\nh\ns\nh\ns\nh\ns\nh\ns\nh\ns\n", input);
        std::rewind(input);
        config = AppConfig{.player = PLAYER_CAUTIOUS, .threshold = 12, .deckCount = 1};
    }

    void TearDown() override
    {
        std::fclose(input);
    }

    HeapFake heap;
    NiceMock<mocks::LoggerMock> logger;
    FILE* input{};
    AppConfig config{};
    Score score{};
};
} // namespace

TEST_F(AppMemory, AppPlayAllocatesMemoryThroughHeapFake)
{
    // when
    appPlay(&config, 1, input, &logger.mock, &score);

    // then
    EXPECT_THAT(heap.AllocationsCount(), Gt(0u));
}

TEST_F(AppMemory, AppPlayFreesAllMemoryWhenThresholdPlayer)
{
    // when
    appPlay(&config, 1, input, &logger.mock, &score);

    // then
    EXPECT_THAT(heap.NotFreedBlocks(), IsEmpty());
}

TEST_F(AppMemory, AppPlayFreesAllMemoryWhenConsolePlayer)
{
    // given
    config.player = PLAYER_CONSOLE;
    config.threshold = 0;

    // when
    appPlay(&config, 1, input, &logger.mock, &score);

    // then
    EXPECT_THAT(heap.NotFreedBlocks(), IsEmpty());
}

TEST_F(AppMemory, AppPlayFreesAllMemoryWhenManyDecks)
{
    // given
    config.deckCount = 6;

    // when
    appPlay(&config, 1, input, &logger.mock, &score);

    // then
    EXPECT_THAT(heap.NotFreedBlocks(), IsEmpty());
}

TEST_F(AppMemory, AppPlayFreesAllMemoryWhenLoggerIsNull)
{
    // when
    appPlay(&config, 1, input, nullptr, &score);

    // then
    EXPECT_THAT(heap.NotFreedBlocks(), IsEmpty());
}

TEST_F(AppMemory, AppPlayFreesNothingTwice)
{
    // when
    appPlay(&config, 1, input, &logger.mock, &score);

    // then
    EXPECT_THAT(heap.FreedManyTimesBlocks(), IsEmpty());
}

TEST_F(AppMemory, AppPlayFreesOnlyMemoryItAllocated)
{
    // when
    appPlay(&config, 1, input, &logger.mock, &score);

    // then
    EXPECT_THAT(heap.UnknownFreedPointers(), IsEmpty());
}

TEST_F(AppMemory, AppPlayFreesAllMemoryWhenDeckCannotBeCreated)
{
    // given
    config.deckCount = 0;

    // when
    appPlay(&config, 1, input, &logger.mock, &score);

    // then
    EXPECT_THAT(heap.NotFreedBlocks(), IsEmpty());
}

TEST_F(AppMemory, AppRunFreesAllMemory)
{
    // when
    appRun(&config, 1, input, &logger.mock);

    // then
    EXPECT_THAT(heap.NotFreedBlocks(), IsEmpty());
}

TEST_F(AppMemory, AppRunFreesNothingTwice)
{
    // when
    appRun(&config, 1, input, &logger.mock);

    // then
    EXPECT_THAT(heap.FreedManyTimesBlocks(), IsEmpty());
}
} // namespace test
