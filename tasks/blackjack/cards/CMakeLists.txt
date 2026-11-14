# Header-only module. When you add Card.c (stage 1), turn it into a STATIC library,
set(LIB_NAME CCards)

add_library(${LIB_NAME} INTERFACE)
add_library(Blackjack::${LIB_NAME} ALIAS ${LIB_NAME})

target_include_directories(${LIB_NAME} INTERFACE .)
