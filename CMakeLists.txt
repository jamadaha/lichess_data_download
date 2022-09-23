cmake_minimum_required(VERSION 3.0.0)
project(LichessToDB VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

add_executable(LichessToDB main.cpp
"include/cxxopts/cxxopts.hpp"
"include/indicators/indicators.hpp"
"src/Downloader.cpp")

target_link_libraries(LichessToDB ${CURL_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
