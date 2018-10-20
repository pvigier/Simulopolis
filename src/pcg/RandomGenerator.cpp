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

#include "RandomGenerator.h"

RandomGenerator::RandomGenerator(result_type seed) : mSeed(seed), mGenerator(mSeed)
{
    //ctor
}

RandomGenerator::result_type RandomGenerator::getSeed()
{
    return mSeed;
}

void RandomGenerator::setSeed(result_type seed)
{
    mSeed = seed;
    mGenerator.seed(0);
}

RandomGenerator::result_type RandomGenerator::operator()()
{
    return mGenerator();
}
