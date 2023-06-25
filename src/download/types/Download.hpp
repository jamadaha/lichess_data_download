#ifndef DOWNLOAD
#define DOWNLOAD

#include <string>
#include "doctest/doctest.h"

struct Download {
public:
    uint year;
    uint month;
    std::string link;
    std::string fileName;

    Download(uint year, uint month, std::string link) : 
        year(year), 
        month(month), 
        link(link),
        fileName(RetrieveFileName(link)) {};

    std::string RetrieveFileName(std::string downloadLink) {
        return link.substr(link.find_last_of("/") + 1);
    }
};

TEST_CASE("RetrieveFileName") {
    const std::string link = "https://database.lichess.org/standard/lichess_db_standard_rated_2023-05.pgn.zst";
    Download testDownload = Download(0, 0, link);
    CHECK_EQ("lichess_db_standard_rated_2023-05.pgn.zst", testDownload.fileName);    
}

#endif

