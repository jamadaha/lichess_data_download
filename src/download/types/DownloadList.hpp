#ifndef DOWNLOAD_LIST
#define DOWNLOAD_LIST

#include <vector>
#include <string>
#include <algorithm>
#include "doctest/doctest.h"
#include "Download.hpp"

struct DownloadList : public std::vector<Download> {
    // Returns downloads that fall within given range
    // Everything is inclusive, e.g. 2013, 01, 2013, 02, will return two downloads (2013/01 and 2013/02)
    DownloadList GetRange(uint lowerYear, uint lowerMonth, uint upperYear, uint upperMonth) {
        DownloadList newList;

        for (auto download : *this) {
            if (download.year >= lowerYear && download.year < upperYear) {
                if (download.month >= lowerMonth)
                    newList.push_back(download);
            } else if (download.year >= lowerYear && download.year == upperYear) {
                if (download.month >= lowerMonth && download.month <= upperMonth)
                    newList.push_back(download);
            }
        }

        return newList;
    }
    
    // Given a range in the format mm/yyyy-mm/yyyy
    // Returns the downloads within the range, inclusive
    DownloadList GetRange(const std::string &range) {
        return GetRange(std::atoi(range.substr(3, 7).c_str()),
                        std::atoi(range.substr(0, 2).c_str()), 
                        std::atoi(range.substr(11, 15).c_str()), 
                        std::atoi(range.substr(8, 10).c_str()));
    }
};

TEST_CASE("GetRange") {
    DownloadList downloads;
    downloads.push_back(Download(2013, 01, ""));
    downloads.push_back(Download(2013, 02, ""));
    downloads.push_back(Download(2013, 03, ""));
    downloads.push_back(Download(2014, 01, ""));
    downloads.push_back(Download(2014, 02, ""));
    downloads.push_back(Download(2014, 03, ""));

    SUBCASE("Corretly filters") {
        auto fDownloads = downloads.GetRange(2013, 02, 2013, 02);

        CHECK_EQ(1, fDownloads.size());
        CHECK_EQ(2013, fDownloads[0].year);
        CHECK_EQ(02, fDownloads[0].month);
    }

    SUBCASE("Corretly converts string") {
        auto fDownloads = downloads.GetRange("02/2013-02/2013");
        
        CHECK_EQ(1, fDownloads.size());
        CHECK_EQ(2013, fDownloads[0].year);
        CHECK_EQ(02, fDownloads[0].month);
    }
}

#endif

