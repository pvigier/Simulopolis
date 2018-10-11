#include "util/string.h"
#include <algorithm>

#include <iostream>

std::vector<std::string> split(const std::string& string, char delimiter)
{
    std::vector<std::string> tokens;
    auto start = std::begin(string);
    while (start != std::end(string))
    {
        auto end = std::find(start, std::end(string), delimiter);
        tokens.emplace_back(string.substr(start - std::begin(string), end - start));
        start = end;
        if (start != std::end(string))
            ++start;
    }
    return std::move(tokens);
}
