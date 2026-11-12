project(GoogleTestAppCheck LANGUAGES CXX)

find_package(GTest REQUIRED)

aux_source_directory(. TEST_FILES)

add_executable(GoogleTestAppCheck ${TEST_FILES})

target_link_libraries(GoogleTestAppCheck PRIVATE GTest::gmock_main)