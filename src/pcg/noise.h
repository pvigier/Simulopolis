/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
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
double perlin_noise_2d(uint64_t seed, double x, double y);
double fractal_noise_2d(uint64_t seed, double x, double y, unsigned int octaves = 1, double lacunarity = 2.0, double persistence = 0.5);
