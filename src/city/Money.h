#pragma once

#include "util/strong_typedef.h"

STRONG_TYPEDEF(double, Money)

template <typename Archive>
void serialize(Archive &ar, Money &money, const unsigned int version)
{
    ar & static_cast<double&>(money);
}
