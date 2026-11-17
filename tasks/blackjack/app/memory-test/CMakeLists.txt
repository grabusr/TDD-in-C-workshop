set(TEST_NAME CBlackjackAppMemoryTests)

find_package(GTest REQUIRED)
include(GoogleTest)

# Static libraries that make up the application. Their malloc()/free() references are redirected to the HeapFake.
set(HOOKED_LIBS
        CBlackjackApp
        CBlackjackGame
        CBlackjackSimulation
        CCardList
        CCardShuffler
        CCards
        CConsolePlayerStrategy
        CDeckBuilder
        CSeededRandom
        CThresholdPlayerStrategy
)

find_program(OBJCOPY_PROGRAM objcopy REQUIRED)

set(HOOKED_OBJECT ${CMAKE_CURRENT_BINARY_DIR}/CBlackjackAppHooked.o)

set(HOOKED_ARCHIVES "")
foreach (LIB IN LISTS HOOKED_LIBS)
    list(APPEND HOOKED_ARCHIVES $<TARGET_FILE:${LIB}>)
endforeach ()

# 1. Merge the whole application into one relocatable object.
# 2. Rename the malloc/free symbols in it, so the application calls heapFakeMalloc/heapFakeFree defined by the tests.
add_custom_command(
        OUTPUT ${HOOKED_OBJECT}
        COMMAND ${CMAKE_C_COMPILER} -r -nostdlib -o ${HOOKED_OBJECT}
        -Wl,--whole-archive ${HOOKED_ARCHIVES} -Wl,--no-whole-archive
        COMMAND ${OBJCOPY_PROGRAM}
        --redefine-sym malloc=heapFakeMalloc
        --redefine-sym free=heapFakeFree
        ${HOOKED_OBJECT}
        DEPENDS ${HOOKED_LIBS}
        VERBATIM
)

set_source_files_properties(${HOOKED_OBJECT} PROPERTIES EXTERNAL_OBJECT TRUE GENERATED TRUE)

aux_source_directory(. TEST_FILES)

add_executable(${TEST_NAME} ${TEST_FILES} ${HOOKED_OBJECT})

target_link_libraries(${TEST_NAME}
        PRIVATE
        GTest::gmock_main
        Blackjack::CBlackjackApp
)

gtest_discover_tests(${TEST_NAME} PROPERTIES LABELS "memory")
