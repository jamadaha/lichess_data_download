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
# PQXX
###############################################################################

FetchContent_Declare(
    pqxx
    GIT_REPOSITORY https://github.com/jtv/libpqxx
    GIT_TAG 7.7.5
)
FetchContent_MakeAvailable(pqxx)

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
    )

set(EXTRACTION_SRC
    )

set(EXTRACTION ${EXTRACTION_HEADERS} ${EXTRACTION_SRC})

###############################################################################
# Database Connection
###############################################################################

set(DB_HEADERS
        "src/db/DBConnection.hpp"
        "src/db/SQLInsertBuiling.hpp"
        "src/db/postgres/PostgresConnection.hpp"
    )

set(DB_SRC
        "src/db/postgres/PostgresConnection.cpp"
    )

set(DB ${DB_HEADERS} ${DB_SRC})

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

###############################################################################
# LichessToDB
###############################################################################

add_executable(LichessToDB 
    src/bin/LichessToDB/LichessToDB.cpp
    "src/bin/LichessToDB/ArgumentParsing.hpp"
    ${SHARED_INCLUDE}
    ${DB})

target_link_libraries(LichessToDB ${CURL_LIBRARIES} pqxx)
target_include_directories(LichessToDB PRIVATE src third_party)

