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

#include <vector>
#include "util/Vector.h"

class Path
{
public:
    Path();
    Path(std::vector<Vector2f> points, bool loop = false);

    Vector2f getCurrentPoint() const;
    const std::vector<Vector2f> getPoints() const;
    void setNextPoint();
    bool isFinished() const;
    bool isEmpty() const;

private:
    std::vector<Vector2f> mPoints;
    std::size_t mCurPoint;
    bool mLoop;

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mPoints & mCurPoint & mLoop;
    }
};
