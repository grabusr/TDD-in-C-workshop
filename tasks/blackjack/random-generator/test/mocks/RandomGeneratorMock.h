#pragma once

#include <RandomGenerator.h>

#include <gmock/gmock.h>

namespace test::mocks
{

class RandomGeneratorMock
{
public:
    MOCK_METHOD(size_t, next, (size_t min, size_t max));

    RandomGenerator mock{
        .next = &RandomGeneratorMock::nextMockMethod,
        .context = this,
    };

private:
    static size_t nextMockMethod(void* context, size_t min, size_t max)
    {
        return static_cast<RandomGeneratorMock*>(context)->next(min, max);
    }
};
} // namespace test::mocks
