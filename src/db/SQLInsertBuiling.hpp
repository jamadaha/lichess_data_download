#ifndef SQL_INSERT_BUILDING
#define SQL_INSERT_BUILDING

#include <string>
#include <vector>
#include "doctest/doctest.h"
#include "pgn_parsing/types/MatchInfo.hpp"
#include "pgn_parsing/types/MatchResult.hpp"
#include "pgn_parsing/types/MatchTermination.hpp"

namespace SQLInsertBuilding {
    static std::string GenerateMatchInsert(const MatchInfo &match, const std::string &table) {
        std::string statement = "INSERT INTO ";
        statement += "\"" + table + "\"";
        std::vector<std::pair<std::string, std::string>> cols;
        cols.push_back({"site", match.site});
        cols.push_back({"date", match.date});
        cols.push_back({"time", match.time});
        cols.push_back({"match_type", MatchResults[(uint) match.result]});
        cols.push_back({"match_termination", MatchTerminations[(uint) match.termination]});
        if (match.whitePlayer.has_value())
            cols.push_back({"white_player", match.whitePlayer.value()});
        if (match.blackPlayer.has_value())
            cols.push_back({"black_player", match.blackPlayer.value()});
        if (match.whiteElo.has_value())
            cols.push_back({"white_elo", std::to_string(match.whiteElo.value())});
        if (match.blackElo.has_value())
            cols.push_back({"black_elo", std::to_string(match.blackElo.value())});
        cols.push_back({"moves", match.moves});

        statement += "(";

        for (uint i = 0; i < cols.size(); ++i) {
            statement += cols.at(i).first;
            if (i != cols.size() - 1)
                statement += ',';
        }

        statement += ") VALUES (";

        for (uint i = 0; i < cols.size(); ++i) {
            statement += "'" + cols.at(i).second + "'";
            if (i != cols.size() - 1)
                statement += ',';
        }

        statement += ");";

        return statement;
    }

    namespace Test {
        TEST_CASE("GenerateMatchInsert") {
            SUBCASE("MinCase") {
                MatchInfo match{MatchType::Blitz, "testSite", {}, {}, MatchResult::Unfinished, "2013-01-01", "00:00:00", {}, {}, MatchTermination::Unterminated, "1. e4"};
                std::string tableName = "table";
                std::string statement = GenerateMatchInsert(match, tableName);
                CHECK_EQ("INSERT INTO \"table\"(site,date,time,match_type,match_termination,moves) VALUES "
                         "('testSite','2013-01-01','00:00:00','Unfinished','Unterminated','1. e4');", statement);
            }
            SUBCASE("FullCase") {
                MatchInfo match{MatchType::Blitz, "testSite", "A", "B", MatchResult::Unfinished, "2013-01-01", "00:00:00", 0, 1, MatchTermination::Unterminated, "1. e4"};
                std::string tableName = "table";
                std::string statement = GenerateMatchInsert(match, tableName);
                CHECK_EQ("INSERT INTO \"table\""
                         "(site,date,time,match_type,match_termination,white_player,black_player,white_elo,black_elo,moves)"
                         " VALUES "
                         "('testSite','2013-01-01','00:00:00','Unfinished','Unterminated','A','B','0','1','1. e4');", statement);
            }
        }
    }
}

#endif

