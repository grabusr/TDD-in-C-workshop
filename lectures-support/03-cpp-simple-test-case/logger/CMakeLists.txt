set(LIB_NAME Logger)

aux_source_directory(. SRC_FILES)

add_library(${LIB_NAME} STATIC ${SRC_FILES})
add_library(Messenger::${LIB_NAME} ALIAS ${LIB_NAME})

target_include_directories(${LIB_NAME} INTERFACE .)
