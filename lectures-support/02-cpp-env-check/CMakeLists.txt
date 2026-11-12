cmake_minimum_required(VERSION 4.3)
project(CppEnvCheck LANGUAGES CXX)

aux_source_directory(. SRC_FILES)

add_executable(CppEnvCheckApp ${SRC_FILES})
