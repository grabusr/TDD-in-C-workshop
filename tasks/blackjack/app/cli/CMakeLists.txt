set(APP_NAME BlackjackApp)

add_executable(${APP_NAME} main.c)

target_link_libraries(${APP_NAME}
        PRIVATE
        Blackjack::CBlackjackApp
        Blackjack::CConsoleLogger
)
