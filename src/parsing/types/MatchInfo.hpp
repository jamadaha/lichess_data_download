#ifndef MATCH_INFO
#define MATCH_INFO

#include <string>
#include <optional>
#include "MatchType.hpp"
#include "MatchResult.hpp"
#include "MatchTermination.hpp"

struct MatchInfo {
    MatchType type;
    std::string site;
    std::optional<std::string> whitePlayer;
    std::optional<std::string> blackPlayer;
    MatchResult result;
    std::string date;
    std::string time;
    std::optional<uint> whiteElo;
    std::optional<uint> blackElo;
    MatchTermination termination;
    MatchInfo(
            MatchType type, 
            std::string site, 
            std::optional<std::string> whitePlayer, 
            std::optional<std::string> blackPlayer,
            MatchResult result,
            std::string date,
            std::string time,
            std::optional<uint> whiteElo,
            std::optional<uint> blackElo,
            MatchTermination termination) :
        type(type),
        site(site),
        whitePlayer(whitePlayer),
        blackPlayer(blackPlayer),
        result(result),
        date(date),
        time(time),
        whiteElo(whiteElo),
        blackElo(blackElo),
        termination(termination) {}
};

#endif

