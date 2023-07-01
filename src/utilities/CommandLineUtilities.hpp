#ifndef COMMAND_LINE_UTILITIES
#define COMMAND_LINE_UTILITIES

#include <iostream>
#include "indicators/indicators.hpp"

namespace Utilities {
    static std::ostream& BoldOn(std::ostream& os) {
        return os << "\e[1m";
    }

    static std::ostream& BoldOff(std::ostream& os) {
        return os << "\e[0m";
    }

    static indicators::ProgressBar GenerateProgressBar() {
        using namespace indicators;
        return ProgressBar {
            option::BarWidth{50},
                option::Start{"["},
                option::Fill{"■"},
                option::Lead{"■"},
                option::Remainder{"-"},
                option::End{"]"},
                option::ShowPercentage{true},
                option::ShowElapsedTime{true}
        };
    }
};

#endif
