find_package(GTest REQUIRED)

add_library(CPlayerStrategyMock INTERFACE)
add_library(Blackjack::CPlayerStrategyMock ALIAS CPlayerStrategyMock)

target_include_directories(CPlayerStrategyMock INTERFACE mocks)

target_link_libraries(CPlayerStrategyMock
        INTERFACE
            GTest::gmock
            Blackjack::CPlayerStrategy
)
