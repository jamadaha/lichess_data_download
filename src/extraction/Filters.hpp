#ifndef FILTER_CHECKER
#define FILTER_CHECKER

#include <optional>
#include "Filter.hpp"
#include "pgn_parsing/types/MatchInfo.hpp"
#include "pgn_parsing/types/MatchTermination.hpp"

struct Filters {
    Filters(std::optional<uint> minRating, std::optional<uint> maxRating, 
            std::optional<MatchTermination> termination) :
        minRating(minRating), maxRating(maxRating), termination(termination) {}

    bool operator()(const MatchInfo &match) const {
        if (minRating.has_value()   && !minRating.value()  (match)) return false;
        if (maxRating.has_value()   && !maxRating.value()  (match)) return false;
        if (termination.has_value() && !termination.value()(match)) return false;
        return true;
    }
    
private:
    const std::optional<Filter::MinRating> minRating;
    const std::optional<Filter::MaxRating> maxRating;
    const std::optional<Filter::Termination> termination;
};

#endif

