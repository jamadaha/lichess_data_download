#ifndef UTILITIES
#define UTILITIES

#include <iostream>

class Utilities {
public:
    static std::ostream& BoldOn(std::ostream& os)
    {
        return os << "\e[1m";
    }

    static std::ostream& BoldOff(std::ostream& os)
    {
        return os << "\e[0m";
    }
};

#endif
