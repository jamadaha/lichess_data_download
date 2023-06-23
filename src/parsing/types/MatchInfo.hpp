#ifndef MATCH_INFO
#define MATCH_INFO

#include <string>
#include "MatchType.hpp"
#include "MatchResult.hpp"
#include "MatchTermination.hpp"

struct MatchInfo {
    MatchType type;
    std::string url;
    std::string whitePlayer;
    std::string blackPlayer;
    MatchResult result;
    std::string date;
    std::string time;
    uint whiteELO;
    uint blackELO;
    MatchTermination termination;
};

#endif

