#pragma once

/**
 * \brief Abstract class that allows another class to be non movable
 * \author Pierre Vigier
 */
class NonMovable
{
public:
    NonMovable() = default;
    NonMovable(NonCopyable&&) = delete;
    NonCopyable& operator=(NonCopyable&&) = delete;
};
