#include "regex.h"
#include <array>
#include <iostream>

std::string regexNumbersUntil(uint64_t n)
{
    // Base case
    if (n < 10)
        return "[0-" + std::to_string(n) + "]";
    // Find the digits
    std::array<int, 20> digits;
    std::size_t i = 0;
    while (n != 0)
    {
        digits[i] = n % 10;
        n /= 10;
        ++i;
    }
    // Generate the regex
    std::string s = "\\d{1," + std::to_string(i - 1) + "}";
    std::string prefix = "";
    for (std::size_t j = 0; j < i - 1; ++j)
    {
        prefix += std::to_string(digits[i - j - 1]);
        if (digits[i - j - 2] > 0)
        {
            s += "|" + prefix;
            s += "[0-" + std::to_string(digits[i - j - 2] - 1) + "]";
            s += "\\d{" + std::to_string(i - j - 2) + "}";
        }
    }
    prefix += std::to_string(digits[0]);
    s += "|" + prefix;

    return s;
}
