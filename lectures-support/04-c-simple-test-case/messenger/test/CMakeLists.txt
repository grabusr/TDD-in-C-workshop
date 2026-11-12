set(TEST_NAME CMessengerTests)

find_package(GTest REQUIRED)

aux_source_directory(. TEST_FILES)

add_executable(${TEST_NAME} ${TEST_FILES})

target_link_libraries(${TEST_NAME}
        PRIVATE
            GTest::gmock_main
            Messenger::CMessengerLib
)
target_link_libraries(${TEST_NAME}
        PRIVATE
            Messenger::CLoggerFake
)
