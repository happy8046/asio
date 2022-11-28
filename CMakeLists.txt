cmake_minimum_required(VERSION 2.6)

project(asio)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

aux_source_directory(src SROURCE_FILES)

# add_library(${PROJECT_NAME} STATIC ${SROURCE_FILES})

add_executable(test ${SROURCE_FILES})