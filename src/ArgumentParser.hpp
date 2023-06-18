#ifndef ARGUMENT_HANDLER
#define ARGUMENT_HANDLER

#include <string>
#include "../include/cxxopts/cxxopts.hpp"

struct Arguments {
    const std::string tempPath;
    const std::string downloadPath;
    const std::string extractPath;
    const std::string range;
    const bool extract;
    Arguments(std::string path, std::string range, bool extract) : 
        tempPath(path),
        downloadPath(path + "/Downloads/"),
        extractPath(path + "/Extracts/"),
        range(range),
        extract(extract) {}
};

struct ArgumentParser {
    static Arguments Parse(int argc, char** argv) {
        cxxopts::Options options("LichessToDB", "Downloads Lichess data, extracts it, then inserts into local Postgres instance");

        options.add_options()
            ("h,help", "Print usage")
            ("p,path", "Temporary path for download and extraction", cxxopts::value<std::string>()->default_value("./Temp"))
            ("r,range", "Download all months within the spcified range", cxxopts::value<std::string>()->default_value("01/2013-01/2013"))
            ("e,extract", "Extract downloaded files", cxxopts::value<bool>()->default_value("1"))
        ;

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl; 
            exit(0);
        }

        try {
            return Arguments(
                    result["p"].as<std::string>(),
                    result["r"].as<std::string>(),
                    result["e"].as<bool>()
                    );
        } catch (const cxxopts::missing_argument_exception &e) {
            printf("\nMissing argument: %s\n", e.what());
            printf("Run program with -h or --help to see required arguments.\n");
            printf("Specifically those without a default value\n");
            exit(1);
        }
    }
};

#endif
