set(LIB_NAME CDeckBuilder)

aux_source_directory(. SRC_FILES)

add_library(${LIB_NAME} STATIC ${SRC_FILES})
add_library(Blackjack::${LIB_NAME} ALIAS ${LIB_NAME})

target_include_directories(${LIB_NAME} INTERFACE .)

target_link_libraries(${LIB_NAME}
        PUBLIC
        Blackjack::CCardList
)

add_subdirectory(test)
