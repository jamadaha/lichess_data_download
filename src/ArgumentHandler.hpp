#ifndef ARGUMENT_HANDLER
#define ARGUMENT_HANDLER

#include <string>
#include "../include/cxxopts/cxxopts.hpp"

class ArgumentHandler {
public:
    std::string tempPath;
    std::string downloadPath;
    std::string extractPath;
    std::string range;
    bool doExtract;

    int GetOptions(int argc, char** argv) {
        cxxopts::Options options("LichessToDB", "Downloads Lichess data, extracts it, then inserts into local Postgres instance");

        options.add_options()
            ("h,help", "Print usage")
            ("p,path", "Temporary path for download and extraction", cxxopts::value<std::string>()->default_value("./Temp"))
            ("r,range", "Download all months within the spcified range", cxxopts::value<std::string>()->default_value("01/2013-01/2013"))
            ("e,extract", "Extract downloaded files", cxxopts::value<int>()->default_value("1"))
        ;

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl; 
            return 1;
        }

        tempPath = result["p"].as<std::string>();
        downloadPath = tempPath + "/Downloads/";
        extractPath = tempPath + "/Extracts/";
        range = result["r"].as<std::string>();
        doExtract = (bool) result["e"].as<int>();
        return 0;
    }
};

#endif
