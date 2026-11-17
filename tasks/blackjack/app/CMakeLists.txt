set(LIB_NAME CBlackjackApp)

aux_source_directory(. SRC_FILES)

add_library(${LIB_NAME} STATIC ${SRC_FILES})
add_library(Blackjack::${LIB_NAME} ALIAS ${LIB_NAME})

target_include_directories(${LIB_NAME} INTERFACE .)

target_link_libraries(${LIB_NAME}
        PUBLIC
        Blackjack::CBlackjackGame
        Blackjack::CLogger
        PRIVATE
        Blackjack::CDeckBuilder
        Blackjack::CSeededRandom
        Blackjack::CCardShuffler
        Blackjack::CThresholdPlayerStrategy
        Blackjack::CConsolePlayerStrategy
        Blackjack::CBlackjackSimulation
)

add_subdirectory(test)
add_subdirectory(cli)
