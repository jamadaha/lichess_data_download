#include "Filter.hpp"
#include "pgn_parsing/types/MatchInfo.hpp"

bool Filter::MinRating::operator()(const MatchInfo &match) const {
    if (!match.whiteElo.has_value() || match.whiteElo.value() < rating)
        return false;
    if (!match.blackElo.has_value() || match.blackElo.value() < rating)
        return false;
    return true;
}

bool Filter::MaxRating::operator()(const MatchInfo &match) const {
    if (match.whiteElo.has_value() && match.whiteElo.value() > rating)
        return false;
    if (match.blackElo.has_value() && match.blackElo.value() > rating)
        return false;
    return true;
}

