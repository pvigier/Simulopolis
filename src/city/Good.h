#pragma once

#include <string>

enum class Good : int {NECESSARY = 0, NORMAL, LUXURY};

std::string goodToString(Good good);
