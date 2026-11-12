set(TEST_NAME ${LIB_NAME}Tests)

find_package(GTest REQUIRED)

aux_source_directory(. TEST_FILES)

add_executable(${TEST_NAME} ${TEST_FILES})

target_link_libraries(${TEST_NAME}
        PRIVATE
            Messenger::${LIB_NAME}
            GTest::gmock_main
)
