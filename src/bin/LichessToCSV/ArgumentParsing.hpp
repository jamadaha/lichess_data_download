#ifndef ARGUMENT_HANDLER
#define ARGUMENT_HANDLER

#include <string>
#include "doctest/doctest.h"
#include "cxxopts/cxxopts.hpp"
#include "extraction/Filters.hpp"
#include "pgn_parsing/types/MatchTermination.hpp"

struct Arguments {
    const std::string range;
    const std::string tempPath;
    const std::string downloadPath;
    const std::string extractPath;
    const std::string csvPath;
    const Filters filters;
    Arguments(std::string range, std::string tempPath, std::string path, Filters filters) : 
        range(range),
        tempPath(tempPath),
        downloadPath(tempPath + "/downloads/"),
        extractPath(tempPath + "/extracts/"),
        csvPath(path),
        filters(filters){}
};

namespace ArgumentParsing {
    static Arguments Parse(int argc, char** argv) {
        cxxopts::Options options("LichessToDB", "Downloads Lichess data, extracts it, then inserts into local Postgres instance");

        options.add_options()
            ("h,help", "Print usage")
            ("t,test", "Runs test")
            ("r,range", "Download all months within the spcified range", cxxopts::value<std::string>()->default_value("01/2013-01/2013"))
            ("temp_path", "Temporary path for download and extraction", cxxopts::value<std::string>()->default_value("./temp"))
            ("p,path", "Path for extracted data", cxxopts::value<std::string>()->default_value("./data.csv"))
            ("min_rating", "Minimum player rating", cxxopts::value<uint>())
            ("max_rating", "Maximum player rating", cxxopts::value<uint>())
            ("termination", "Allowed termination type\n" + GenerateMatchTerminationDesc(), cxxopts::value<uint>())
        ;

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl; 
            exit(0);
        } else if (result.count("test")) {
            doctest::Context context;
            context.setOption("order-by", "name");
            int res = context.run();
            exit(res);
        }

        try {
            return Arguments(
                    result["range"].as<std::string>(),
                    result["temp_path"].as<std::string>(),
                    result["path"].as<std::string>(),
                    Filters(
                        result.count("min_rating") ? result["min_rating"].as<uint>() : std::optional<uint>(),
                        result.count("max_rating") ? result["max_rating"].as<uint>() : std::optional<uint>(),
                        result.count("termination") ? (MatchTermination) result["termination"].as<uint>() : std::optional<MatchTermination>()
                        )
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
