set(FAKE_NAME ${LIB_NAME}Fake)

aux_source_directory(. FAKE_FILES)

add_library(${FAKE_NAME} STATIC ${FAKE_FILES})
add_library(Messenger::${FAKE_NAME} ALIAS ${FAKE_NAME})

target_link_libraries(${FAKE_NAME} PUBLIC Messenger::${API_NAME})
