#pragma once

#include <memory>
#include <string>
#include <cstdio>

template<typename ... Args>
std::string format(const std::string& format, Args... args)
{
    std::size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1;
    std::unique_ptr<char[]> buffer(new char[size]);
    snprintf(buffer.get(), size, format.c_str(), args...);
    return std::string(buffer.get(), buffer.get() + size - 1);
}
