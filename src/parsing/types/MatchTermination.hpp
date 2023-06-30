#ifndef MATCH_TERMINATION
#define MATCH_TERMINATION

#include <string>

enum class MatchTermination {
    Normal,
    TimeForfeit,
    Abandoned,
    RulesInfraction,
    Unterminated
};

const static inline std::string MatchTerminations[5] {
    "Normal",
    "TimeForfeit",
    "Abandoned",
    "RulesInfranction",
    "Unterminated"
};

#endif

