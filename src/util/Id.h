#pragma once

#include <limits>

/**
 * This definition is particurlarly used by the IdManager
 */
typedef std::size_t Id;
constexpr Id UNDEFINED = std::numeric_limits<Id>::max();
