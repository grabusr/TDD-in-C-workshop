set(LIB_NAME CRandomGenerator)

add_library(${LIB_NAME} INTERFACE)
add_library(Blackjack::${LIB_NAME} ALIAS ${LIB_NAME})

target_include_directories(${LIB_NAME} INTERFACE .)

