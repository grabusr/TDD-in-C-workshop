set(APP_NAME CppMessengerApp)

aux_source_directory(. SRC_FILES)

add_executable(${APP_NAME} ${SRC_FILES})

target_link_libraries(${APP_NAME}
        PRIVATE
            Messenger::CppMessengerLib
)
