set(APP_NAME CMessengerApp)

add_executable(${APP_NAME} main.c)

target_link_libraries(${APP_NAME}
        PRIVATE
            Messenger::CMessengerLib
            Messenger::CService
            Messenger::CLoggerImpl
)
