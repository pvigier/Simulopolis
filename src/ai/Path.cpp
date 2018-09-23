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

#include "Path.h"

Path::Path() : mCurPoint(0), mLoop(false)
{

}

Path::Path(std::vector<Vector2f> points, bool loop) : mPoints(std::move(points)), mCurPoint(0), mLoop(loop)
{
    //ctor
}

Vector2f Path::getCurrentPoint() const
{
    return mPoints[mCurPoint];
}

Vector2f Path::getLastPoint() const
{
    return mPoints.back();
}

const std::vector<Vector2f> Path::getPoints() const
{
    return mPoints;
}

void Path::setNextPoint()
{
    ++mCurPoint;
    if (mLoop && mCurPoint >= mPoints.size())
        mCurPoint = 0;
}

bool Path::isFinished() const
{
    return !mLoop && mCurPoint == (mPoints.size() - 1);
}

bool Path::isEmpty() const
{
    return mPoints.empty();
}
