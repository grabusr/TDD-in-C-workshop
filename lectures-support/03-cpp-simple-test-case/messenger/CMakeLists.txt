set(LIB_NAME CppMessengerLib)

aux_source_directory(. SRC_FILES)

add_library(${LIB_NAME} STATIC ${SRC_FILES})
add_library(Messenger::${LIB_NAME} ALIAS ${LIB_NAME})

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME}
        PUBLIC
            Messenger::Logger
            Messenger::Service
)

add_subdirectory(test)
