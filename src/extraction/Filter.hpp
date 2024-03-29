#ifndef FILTER
#define FILTER

#include "pgn_parsing/types/MatchInfo.hpp"
#include "pgn_parsing/types/MatchTermination.hpp"

namespace Filter {
    struct Filter {
        virtual bool operator()(const MatchInfo &match) const = 0;
        virtual ~Filter() = default;
    };

    struct MinRating : public Filter {
        explicit MinRating(uint rating) : rating(rating) {}
        bool operator()(const MatchInfo &match) const final;
    private:
        const uint rating;
    };

    struct MaxRating : public Filter {
        explicit MaxRating(uint rating) : rating(rating) {}
        bool operator()(const MatchInfo &match) const final;
    private:
        const uint rating;
    };

    struct Termination : public Filter {
        explicit Termination(MatchTermination termination) : termination(termination){}
        bool operator()(const MatchInfo &match) const final;
    private:
        const MatchTermination termination;
    };
}

#endif

