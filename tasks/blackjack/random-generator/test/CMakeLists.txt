find_package(GTest REQUIRED)

add_library(CRandomGeneratorMock INTERFACE)
add_library(Blackjack::CRandomGeneratorMock ALIAS CRandomGeneratorMock)

target_include_directories(CRandomGeneratorMock INTERFACE mocks)

target_link_libraries(CRandomGeneratorMock
        INTERFACE
        GTest::gmock
        Blackjack::CRandomGenerator
)
