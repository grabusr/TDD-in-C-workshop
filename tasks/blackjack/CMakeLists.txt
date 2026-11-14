cmake_minimum_required(VERSION 4.3)
project(TddBlackjackTasks LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(cards)
add_subdirectory(deck-parser)
