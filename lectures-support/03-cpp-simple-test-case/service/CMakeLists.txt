set(LIB_NAME Service)

add_library(${LIB_NAME} STATIC ConsolePrinter.cpp)
add_library(Messenger::${LIB_NAME} ALIAS ${LIB_NAME})

target_include_directories(${LIB_NAME} PUBLIC .)
