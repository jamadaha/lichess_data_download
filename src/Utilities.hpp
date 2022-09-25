#ifndef UTILITIES
#define UTILITIES

#include <iostream>

namespace Utilities {
    std::ostream& BoldOn(std::ostream& os)
    {
        return os << "\e[1m";
    }

    std::ostream& BoldOff(std::ostream& os)
    {
        return os << "\e[0m";
    }
};

#endif
