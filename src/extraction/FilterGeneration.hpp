#ifndef FILTER_GENERATION
#define FILTER_GENERATION

#include <vector>
#include <optional>
#include "Filter.hpp"

namespace FilterGeneration {
    static std::vector<Filter::Filter*> GenerateFilters(const std::optional<uint> &minRating, const std::optional<uint> &maxRating) {
        std::vector<Filter::Filter*> filters;

        if (minRating.has_value())
            filters.push_back(new Filter::MinRating(minRating.value()));
        if (maxRating.has_value())
            filters.push_back(new Filter::MaxRating(maxRating.value()));

        return filters;
    }

};

#endif

