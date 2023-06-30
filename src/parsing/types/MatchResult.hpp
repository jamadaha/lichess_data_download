#ifndef MATCHRESULT
#define MATCHRESULT

#include <string>

enum class MatchResult {
    WhiteWin,
    BlackWin,
    Draw,
    Unfinished
};

const static inline std::string MatchResults[4] {
    "WhiteWin",
    "BlackWin",
    "Draw",
    "Unfinished"
};

#endif

