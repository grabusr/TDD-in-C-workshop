cmake_minimum_required(VERSION 4.3)
project(TddBlackjackTasks LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(cards)
add_subdirectory(deck-parser)
add_subdirectory(random-generator)
add_subdirectory(card-shuffler)
add_subdirectory(game-model)
add_subdirectory(player-strategy)
add_subdirectory(threshold-strategy)
add_subdirectory(simulation)

add_custom_target(RunAllTests
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
    COMMAND ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_BINARY_DIR} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    USES_TERMINAL
    COMMENT "Building all targets and running all CTest tests"
)
