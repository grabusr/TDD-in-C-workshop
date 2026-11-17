set(TEST_NAME CDeckBuilderTests)

find_package(GTest REQUIRED)
include(GoogleTest)

aux_source_directory(. TEST_FILES)

add_executable(${TEST_NAME} ${TEST_FILES})

target_link_libraries(${TEST_NAME}
        PRIVATE
        GTest::gmock_main
        Blackjack::CDeckBuilder
)

gtest_discover_tests(${TEST_NAME})
