set(LIB_NAME CPlayerStrategy)

add_library(${LIB_NAME} INTERFACE)
add_library(Blackjack::${LIB_NAME} ALIAS ${LIB_NAME})

target_include_directories(${LIB_NAME} INTERFACE .)

target_link_libraries(${LIB_NAME}
        INTERFACE
            Blackjack::CBlackjackGame
            Blackjack::CCards
)

