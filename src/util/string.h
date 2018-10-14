#pragma once

#include <vector>
#include <string>

std::vector<std::string> split(const std::string& string, char delimiter);

template <typename Container, typename Separator>
std::string join(const Container& tokens, Separator separator)
{
    if (tokens.empty())
        return "";
    auto it = std::begin(tokens);
    std::string result = *it;
    for (++it; it != std::end(tokens); ++it)
        result += separator + *it;
    return result;
}
