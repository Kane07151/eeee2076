#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include "adder.h"

static bool parseInt(const char* s, int& out) {
    errno = 0;
    char* end = nullptr;
    long v = std::strtol(s, &end, 10);
    if (errno != 0 || end == s || *end != '\0' || v < INT_MIN || v > INT_MAX) return false;
    out = (int)v;
    return true;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: calc <num1> <num2>" << std::endl;
        return 1;
    }

    int a, b;
    if (!parseInt(argv[1], a) || !parseInt(argv[2], b))
    {
        std::cout << "Error: both arguments must be integers." << std::endl;
        return 1;
    }

    std::cout << add(a, b) << std::endl;
    return 0;
}
