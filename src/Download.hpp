#ifndef DOWNLOAD
#define DOWNLOAD

#include <string>

struct Download {
public:
    int year;
    int month;
    std::string link;

    Download(int year, int month, std::string link) : year(year), month(month), link(link) {};
};

#endif
