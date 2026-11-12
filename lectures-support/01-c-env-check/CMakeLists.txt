cmake_minimum_required(VERSION 4.3)
project(CEnvCheck LANGUAGES C)

aux_source_directory(. SRC_FILES)

add_executable(CEnvCheckApp ${SRC_FILES})

add_subdirectory(test)