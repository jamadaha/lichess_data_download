cmake_minimum_required(VERSION 3.2.4)
project(LichessToDB VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

include(CTest)
enable_testing()

###############################################################################
# CURL
###############################################################################

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

###############################################################################
# Download
###############################################################################

set(DOWNLOAD_HEADERS
        "src/download/types/Download.hpp"
        "src/download/types/DownloadList.hpp"
        "src/download/DataDownloading.hpp"
        "src/download/DownloadParsing.hpp"
    )

set(DOWNLOAD_SRC
        "src/download/Downloader.cpp"
    )

set(DOWNLOAD ${DOWNLOAD_HEADERS} ${DOWNLOAD_SRC})

###############################################################################
# PGN Parsing
###############################################################################

set(PGN_PARSING_HEADERS
        "src/pgn_parsing/types/MatchType.hpp"
        "src/pgn_parsing/types/MatchResult.hpp"
        "src/pgn_parsing/types/MatchInfo.hpp"
        "src/pgn_parsing/types/MatchTermination.hpp"
        "src/pgn_parsing/MatchParsing.hpp"
    )

set(PGN_PARSING_SRC
    )

set(PGN_PARSING ${PGN_PARSING_HEADERS} ${PGN_PARSING_SRC})

###############################################################################
# Extraction
###############################################################################

set(EXTRACTION_HEADERS
    "src/extraction/FileExtraction.hpp"
    "src/extraction/Filter.hpp"
    "src/extraction/Filters.hpp"
    )

set(EXTRACTION_SRC
    "src/extraction/Filter.cpp"
    )

set(EXTRACTION ${EXTRACTION_HEADERS} ${EXTRACTION_SRC})

###############################################################################
# Shared Include
###############################################################################

set(SHARED_INCLUDE
        ${DOWNLOAD}
        ${PGN_PARSING}
        ${EXTRACTION}
        "third_party/cxxopts/cxxopts.hpp"
        "third_party/indicators/indicators.hpp"
        "third_party/doctest/doctest.h"
        "src/utilities/CommandLineUtilities.hpp"
        "src/utilities/FileUtilities.hpp"
    )

###############################################################################
# LichessToCSV
###############################################################################

add_executable(LichessToCSV 
    src/bin/LichessToCSV/LichessToCSV.cpp
    "src/bin/LichessToCSV/ArgumentParsing.hpp"
    ${SHARED_INCLUDE}
    )

target_link_libraries(LichessToCSV ${CURL_LIBRARIES})
target_include_directories(LichessToCSV PRIVATE src third_party)


