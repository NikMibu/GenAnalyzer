cmake_minimum_required(VERSION 3.10)
project(GenAnalyzer)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB SOURCES "src/*.cpp" "lib/*.cpp")

add_executable(GenAnalyzer ${SOURCES})
