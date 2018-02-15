#pragma once

/**
 * \brief Abstract class that allows another class to be non copyable
 *
 * \author Pierre Vigier
 */
class NonCopyable
{
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};
