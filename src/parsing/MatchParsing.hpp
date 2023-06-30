#ifndef MATCH_PARSING
#define MATCH_PARSING

#include <algorithm>
#include <cassert>
#include <cctype>
#include <stdexcept>
#include <vector>
#include <string>
#include "doctest/doctest.h"
#include "parsing/types/MatchResult.hpp"
#include "parsing/types/MatchTermination.hpp"
#include "types/MatchInfo.hpp"
#include "types/MatchType.hpp"

namespace MatchParsing {
    static std::string ParseGeneric(std::string line) {
        uint firstQuote = line.find("\"") + 1;
        uint lastQuote = line.find_last_of("\"");
        uint diff = lastQuote - firstQuote;
        return line.substr(firstQuote, diff);
    }

    static std::string ParseSite(std::string line) {
        auto site = ParseGeneric(line);
        uint idIndex = site.find_last_of('/') + 1;
        return site.substr(idIndex, site.size() - idIndex);
    }

    static std::optional<std::string> ParseUsername(std::string line) {
        auto username = ParseGeneric(line);
        if (username == "?")
            return std::optional<std::string>();
        return username;
    }

    static MatchType ParseMatchType(std::string line) {
        std::transform(line.begin(), line.end(), line.begin(),
                [](char c){ return std::tolower(c); });
        if (line.find("bullet") != std::string::npos)
            return MatchType::Bullet;
        if (line.find("blitz") != std::string::npos)
            return MatchType::Blitz;
        if (line.find("rapid") != std::string::npos)
            return MatchType::Rapid;
        if (line.find("classical") != std::string::npos)
            return MatchType::Classical;
        if (line.find("correspondence") != std::string::npos)
            return MatchType::Correspondence;
        if (line.find("standard") != std::string::npos)
            return MatchType::Standard;
        throw std::invalid_argument("Unexpected match type: " + line);
    }

    static MatchResult ParseMatchResult(std::string line) {
        line = ParseGeneric(line);
        if (line == "1-0")
            return MatchResult::WhiteWin;
        if (line == "0-1")
            return MatchResult::BlackWin;
        if (line == "1/2-1/2")
            return MatchResult::Draw;
        if (line == "*")
            return MatchResult::Unfinished;
        throw std::invalid_argument("Unexpected match result: " + line);
    }

    static std::optional<uint> ParseElo(std::string line) {
        line = ParseGeneric(line);
        if (line == "?")
            return std::optional<uint>();
        else
            return std::atoi(line.c_str());
    }

    static MatchTermination ParseMatchTermination(std::string line) {
        line = ParseGeneric(line); 
        std::transform(line.begin(), line.end(), line.begin(),
                [](char c){ return std::tolower(c); });
        if (line == "normal")
            return MatchTermination::Normal;
        if (line == "time forfeit")
            return MatchTermination::TimeForfeit;
        if (line == "abandoned")
            return MatchTermination::Abandoned;
        if (line == "rules infraction")
            return MatchTermination::RulesInfraction;
        if (line == "unterminated")
            return MatchTermination::Unterminated;
        throw std::invalid_argument("Unexpected match termination: " + line);
    }

    static MatchInfo ParseMatch(std::vector<std::string> lines) {
        std::optional<MatchType> matchType;
        std::optional<std::string> site;
        std::optional<std::string> whitePlayer;
        std::optional<std::string> blackPlayer;
        std::optional<MatchResult> matchResult;
        std::optional<std::string> date;
        std::optional<std::string> time;
        std::optional<uint> whiteElo;
        std::optional<uint> blackElo;
        std::optional<MatchTermination> matchTermination;
        for (auto line : lines) {
            if (line.find("[Event") != std::string::npos)
                matchType = ParseMatchType(line);
            else if (line.find("[Site") != std::string::npos)
                site = ParseSite(line);
            else if (line.find("[White ") != std::string::npos)
                whitePlayer = ParseUsername(line);
            else if (line.find("[Black ") != std::string::npos)
                blackPlayer = ParseUsername(line);
            else if (line.find("[Result") != std::string::npos)
                matchResult = ParseMatchResult(line);
            else if (line.find("[UTCDate") != std::string::npos)
                date = ParseGeneric(line);
            else if (line.find("[UTCTime") != std::string::npos)
                time = ParseGeneric(line);
            else if (line.find("[WhiteElo") != std::string::npos)
                whiteElo = ParseElo(line);
            else if (line.find("[BlackElo") != std::string::npos)
                blackElo = ParseElo(line);
            else if (line.find("[Termination") != std::string::npos)
                matchTermination = ParseMatchTermination(line);
        }
        assert(matchType.has_value());
        assert(site.has_value());
        assert(date.has_value());
        assert(time.has_value());
        assert(matchTermination.has_value());
        assert(matchResult.has_value());
        return MatchInfo(matchType.value(), site.value(), whitePlayer, blackPlayer, 
                matchResult.value(), date.value(), time.value(), whiteElo, blackElo, matchTermination.value());
    }

    namespace Test {
        TEST_CASE("ParseSite") {
            std::string line = "[Site \"https://lichess.org/ri7h2cf8\"]";
            std::string parsedSite = ParseSite(line);
            CHECK_EQ("ri7h2cf8", parsedSite);
        }

        TEST_CASE("ParsePlayer") {
            SUBCASE("ParseWhite") {
                std::string line = "[White \"White\"]";
                std::optional<std::string> player = ParseUsername(line);
                CHECK_EQ("White", player);
            }
            SUBCASE("ParseBlack") {
                std::string line = "[Black \"Black\"]";
                std::optional<std::string> player = ParseUsername(line);
                CHECK_EQ("Black", player);
            }
            SUBCASE("ParseUnknown") {
                std::string line = "[White \"?\"]";
                std::optional<std::string> player = ParseUsername(line);
                CHECK(!player.has_value());
            }
        }

        TEST_CASE("ParseMatchType") {
            SUBCASE("Bullet") {
                std::string line = "[Event \"Rated Bullet game\"]";
                MatchType type = ParseMatchType(line);
                CHECK_EQ(MatchType::Bullet, type);
            }
            SUBCASE("Blitz") {
                std::string line = "[Event \"Rated Blitz game\"]";
                MatchType type = ParseMatchType(line);
                CHECK_EQ(MatchType::Blitz, type);
            }
            SUBCASE("Rapid") {
                std::string line = "[Event \"Rated Rapid game\"]";
                MatchType type = ParseMatchType(line);
                CHECK_EQ(MatchType::Rapid, type);
            }
            SUBCASE("Classical") {
                std::string line = "[Event \"Rated Classical game\"]";
                MatchType type = ParseMatchType(line);
                CHECK_EQ(MatchType::Classical, type);
            }
            SUBCASE("Correspondance") {
                std::string line = "[Event \"Rated Correspondence game\"]";
                MatchType type = ParseMatchType(line);
                CHECK_EQ(MatchType::Correspondence, type);
            }
        }

        TEST_CASE("ParseMatchResult") {
            SUBCASE("WhiteWin") {
                std::string line = "[Result \"1-0\"]";
                MatchResult result = ParseMatchResult(line);
                CHECK_EQ(MatchResult::WhiteWin, result);
            }
            SUBCASE("BlackWin") {
                std::string line = "[Result \"0-1\"]";
                MatchResult result = ParseMatchResult(line);
                CHECK_EQ(MatchResult::BlackWin, result);
            }
            SUBCASE("Draw") {
                std::string line = "[Result \"1/2-1/2\"]";
                MatchResult result = ParseMatchResult(line);
                CHECK_EQ(MatchResult::Draw, result);
            }
        }

        TEST_CASE("ParseElo") {
            SUBCASE("WhiteElo") {
                std::string line = "[WhiteElo \"1234\"]";
                std::optional<uint> elo = ParseElo(line);
                CHECK_EQ(1234, elo);
            }
            SUBCASE("BlackElo") {
                std::string line = "[BlackElo \"1234\"]";
                std::optional<uint> elo = ParseElo(line);
                CHECK_EQ(1234, elo);
            }
        }

        TEST_CASE("ParseMatchTermination") {
            SUBCASE("Normal") {
                std::string line = "[Termination \"Normal\"]";
                MatchTermination terminationType = ParseMatchTermination(line);
                CHECK_EQ(MatchTermination::Normal, terminationType);
            }
            SUBCASE("Time forfeit") {
                std::string line = "[Termination \"Time forfeit\"]";
                MatchTermination terminationType = ParseMatchTermination(line);
                CHECK_EQ(MatchTermination::TimeForfeit, terminationType);
            }
        }

        TEST_CASE("ParseMatch") {
            SUBCASE("WithDiff") {
                std::vector<std::string> lines {
                    "[Event \"Rated Bullet game\"]",
                    "[Site \"https://lichess.org/ri7h2cf8\"]",
                    "[White \"WhitePlayer\"]",
                    "[Black \"BlackPlayer\"]",
                    "[Result \"0-1\"]",
                    "[UTCDate \"2013.04.30\"]",
                    "[UTCTime \"22:00:07\"]",
                    "[WhiteElo \"1663\"]",
                    "[BlackElo \"1635\"]",
                    "[WhiteRatingDiff \"-12\"]",
                    "[BlackRatingDiff \"+13\"]",
                    "[ECO \"A01\"]",
                    "[Opening \"Nimzo-Larsen Attack\"]",
                    "[TimeControl \"0+1\"]",
                    "[Termination \"Normal\"]",
                    "1. b3 e6 2. Bb2 Qe7 3. g4 g6 4. Bxh8 f6 5. g5 b6 6. gxf6 Nxf6 7. Bxf6 Qxf6 8. Nc3 Bb7 9. f3 c6 10. Bg2 Na6 11. e4 Nc7 12. Qe2 O-O-O 13. O-O-O Ba6 14. d3 Qxc3 15. Rd2 Qa1# 0-1"
                };
                MatchInfo matchInfo = ParseMatch(lines);
                CHECK_EQ(MatchType::Bullet, matchInfo.type);
                CHECK_EQ("ri7h2cf8", matchInfo.site);
                CHECK_EQ("WhitePlayer", matchInfo.whitePlayer.value());
                CHECK_EQ("BlackPlayer", matchInfo.blackPlayer.value());
                CHECK_EQ("2013.04.30", matchInfo.date);
                CHECK_EQ("22:00:07", matchInfo.time);
                CHECK_EQ(1663, matchInfo.whiteElo.value());
                CHECK_EQ(1635, matchInfo.blackElo.value());
                CHECK_EQ(MatchTermination::Normal, matchInfo.termination);
            }
            SUBCASE("WithoutDiff") {
                std::vector<std::string> lines {
                    "[Event \"Rated Bullet game\"]",
                    "[Site \"https://lichess.org/ri7h2cf8\"]",
                    "[White \"?\"]",
                    "[Black \"BlackPlayer\"]",
                    "[Result \"0-1\"]",
                    "[UTCDate \"2013.04.30\"]",
                    "[UTCTime \"22:00:07\"]",
                    "[WhiteElo \"?\"]",
                    "[BlackElo \"1635\"]",
                    "[ECO \"A01\"]",
                    "[Opening \"Nimzo-Larsen Attack\"]",
                    "[TimeControl \"0+1\"]",
                    "[Termination \"Normal\"]",
                    "1. b3 e6 2. Bb2 Qe7 3. g4 g6 4. Bxh8 f6 5. g5 b6 6. gxf6 Nxf6 7. Bxf6 Qxf6 8. Nc3 Bb7 9. f3 c6 10. Bg2 Na6 11. e4 Nc7 12. Qe2 O-O-O 13. O-O-O Ba6 14. d3 Qxc3 15. Rd2 Qa1# 0-1"
                };
                MatchInfo matchInfo = ParseMatch(lines);
                CHECK_EQ(MatchType::Bullet, matchInfo.type);
                CHECK_EQ("ri7h2cf8", matchInfo.site);
                CHECK(!matchInfo.whitePlayer.has_value());
                CHECK_EQ("BlackPlayer", matchInfo.blackPlayer.value());
                CHECK_EQ("2013.04.30", matchInfo.date);
                CHECK_EQ("22:00:07", matchInfo.time);
                CHECK(!matchInfo.whiteElo.has_value());
                CHECK_EQ(1635, matchInfo.blackElo);
                CHECK_EQ(MatchTermination::Normal, matchInfo.termination);
            }
        }
    }
}


#endif

