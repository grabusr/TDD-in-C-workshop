set(LIB_NAME CLogger)
set(API_NAME ${LIB_NAME}Api)
set(IMPL_NAME ${LIB_NAME}Impl)

add_library(${API_NAME} INTERFACE)
add_library(Messenger::${API_NAME} ALIAS ${API_NAME})
target_include_directories(${API_NAME} INTERFACE .)

aux_source_directory(. SRC_FILES)

add_library(${IMPL_NAME} STATIC ${SRC_FILES})
add_library(Messenger::${IMPL_NAME} ALIAS ${IMPL_NAME})
target_link_libraries(${IMPL_NAME} PUBLIC Messenger::${API_NAME})

add_subdirectory(test)
