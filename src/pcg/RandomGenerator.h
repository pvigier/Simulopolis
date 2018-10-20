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

#include <random>

class RandomGenerator
{
public:
    using result_type = uint64_t;
    using generator_type = std::mt19937_64;

    static constexpr result_type min()
    {
        return generator_type::min();
    }

    static constexpr result_type max()
    {
        return generator_type::max();
    }

    RandomGenerator(result_type seed = 0);

    result_type getSeed();
    void setSeed(result_type seed);
    result_type operator()();

private:
    result_type mSeed;
    generator_type mGenerator;
};
