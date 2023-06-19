#ifndef DOWNLOAD
#define DOWNLOAD

#include <string>

struct Download {
public:
    const uint year;
    const uint month;
    const std::string link;

    Download(uint year, uint month, std::string link) : year(year), month(month), link(link) {};
};

#endif
