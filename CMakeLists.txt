cmake_minimum_required(VERSION 3.2.4)
project(LichessToDB VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

add_executable(LichessToDB src/main.cpp
"third_party/cxxopts/cxxopts.hpp"
"third_party/indicators/indicators.hpp"
"third_party/doctest/doctest.h"
"src/ArgumentParsing.hpp"
"src/download/types/Download.hpp"
"src/download/types/DownloadList.hpp"
"src/download/DataDownloading.hpp"
"src/download/Downloader.cpp"
"src/download/DownloadParsing.hpp"
"src/parsing/types/MatchType.hpp"
"src/parsing/types/MatchResult.hpp"
"src/parsing/types/MatchInfo.hpp"
"src/parsing/types/MatchTermination.hpp"
"src/parsing/MatchParsing.hpp"
"src/Utilities.hpp")

target_link_libraries(LichessToDB ${CURL_LIBRARIES})
target_include_directories(LichessToDB PRIVATE src third_party)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

