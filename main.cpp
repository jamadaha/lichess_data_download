#include <iostream>
#include <string>
#include "include/cxxopts/cxxopts.hpp"


int main(int argc, char** argv) {
    cxxopts::Options options("LichessToDB", "Downloads Lichess data, extracts it, then inserts into local Postgres instance");

    options.add_options()
        ("h,help", "Print usage")
        ("p,path", "Temporary path for download and extraction", cxxopts::value<std::string>()->default_value("./Temp"))
        ("r,range", "Download all months within the spcified range", cxxopts::value<std::string>()->default_value("01/2013-12/2021"))
        ("e,extract", "Extract downloaded files", cxxopts::value<int>()->default_value("1"))
    ;

    auto result = options.parse(argc, argv);

    //int debug = result["extract"].as<int>();

    return 0;
}
