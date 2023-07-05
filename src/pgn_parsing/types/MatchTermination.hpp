#ifndef MATCH_TERMINATION
#define MATCH_TERMINATION

#include <string>

const static inline uint MatchTerminationCount = 5;

enum class MatchTermination {
    Normal,
    TimeForfeit,
    Abandoned,
    RulesInfraction,
    Unterminated
};

const static inline std::string MatchTerminations[MatchTerminationCount] {
    "Normal",
    "TimeForfeit",
    "Abandoned",
    "RulesInfranction",
    "Unterminated"
};

static std::string GenerateMatchTerminationDesc() {
    std::string desc = "";

    for (uint i = 0; i < MatchTerminationCount; ++i)
        desc += std::to_string(i) + " - " + MatchTerminations[i] + 
            ((i == MatchTerminationCount - 1) ? "" : "\n");

    return desc;
}

#endif

