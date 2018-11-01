#include "util/string.h"
#include <algorithm>

std::vector<std::string> split(const std::string& string, char separator)
{
    std::vector<std::string> tokens;
    auto start = std::begin(string);
    while (start != std::end(string))
    {
        auto end = std::find(start, std::end(string), separator);
        tokens.emplace_back(string.substr(start - std::begin(string), end - start));
        start = end;
        if (start != std::end(string))
            ++start;
    }
    return tokens;
}

std::string strip(const std::string& string)
{
    auto isNotSpace = [](const char& c){ return !std::isspace(c); };
    std::size_t iStart = std::find_if(string.begin(), string.end(), isNotSpace) - string.begin();
    std::size_t iEnd = string.size() - (std::find_if(string.rbegin(), string.rend(), isNotSpace) -
        string.rbegin());
    return string.substr(iStart, iEnd - iStart);
}

std::string toUpper(std::string string)
{
    for (std::size_t i = 0; i < string.size(); ++i)
        string[i] = std::toupper(string[i]);
    return string;
}
