#ifndef SQL_INSERT_BUILDING
#define SQL_INSERT_BUILDING

#include <string>
#include <vector>
#include "doctest/doctest.h"
#include "parsing/types/MatchInfo.hpp"
#include "parsing/types/MatchResult.hpp"
#include "parsing/types/MatchTermination.hpp"

namespace SQLInsertBuilding {
    static std::string GenerateMatchInsert(const MatchInfo &match, const std::string &table) {
        std::string statement = "INSERT INTO ";
        statement += "\"" + table + "\"";
        std::vector<std::pair<std::string, std::string>> cols;
        cols.push_back({"site", match.site});
        cols.push_back({"date", match.date});
        cols.push_back({"time", match.time});
        if (match.whitePlayer.has_value())
            cols.push_back({"white_player", match.whitePlayer.value()});
        if (match.blackPlayer.has_value())
            cols.push_back({"black_player", match.blackPlayer.value()});
        if (match.whiteElo.has_value())
            cols.push_back({"white_elo", std::to_string(match.whiteElo.value())});
        if (match.blackElo.has_value())
            cols.push_back({"black_elo", std::to_string(match.blackElo.value())});

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
                MatchInfo match{MatchType::Blitz, "testSite", {}, {}, MatchResult::Unfinished, "2013-01-01", "00:00:00", {}, {}, MatchTermination::Unterminated};
                std::string tableName = "table";
                std::string statement = GenerateMatchInsert(match, tableName);
                CHECK_EQ("", statement);
            }
        }
    }
}

#endif

