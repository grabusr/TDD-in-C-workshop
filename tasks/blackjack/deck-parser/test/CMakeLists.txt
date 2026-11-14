set(TEST_NAME CCardListTests)

find_package(GTest REQUIRED)
include(GoogleTest)

aux_source_directory(. TEST_FILES)

add_executable(${TEST_NAME} ${TEST_FILES})

# tests read card files from this directory
target_compile_definitions(${TEST_NAME} PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

target_link_libraries(${TEST_NAME}
        PRIVATE
            GTest::gmock_main
            Blackjack::CCardList
)

gtest_discover_tests(${TEST_NAME})
