#pragma once

#include <Logger.h>
#include <string>
#include <vector>

namespace test::fakes::logger
{
/// stores messages send by faked log(...) function
const std::vector<std::string>& messages();

/// clears messages vector
void reset();
} // namespace test::fakeLogger
