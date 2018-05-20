#pragma once

#include <cstdint>
#include <cmath>
#include "util/common.h"

uint64_t integer_noise(uint64_t i);
uint64_t integer_noise_2d(uint64_t i, uint64_t j);
uint64_t integer_noise_3d(uint64_t i, uint64_t j, uint64_t k);
double noise(uint64_t i);
double noise_2d(uint64_t i, uint64_t j);
double noise_3d(uint64_t i, uint64_t j, uint64_t k);
double perlin_noise_2d(double x, double y);
