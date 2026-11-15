#include "AllocatorMock.h"

namespace test::mocks
{

AllocatorMock::AllocatorMock(AllocatorMock*& slot) : slot(slot)
{
    ON_CALL(*this, malloc).WillByDefault(&realMalloc);
    ON_CALL(*this, calloc).WillByDefault(&realCalloc);
    ON_CALL(*this, free).WillByDefault(&realFree);
    slot = this;
}

AllocatorMock::~AllocatorMock()
{
    slot = nullptr;
}

} // namespace test::mocks
