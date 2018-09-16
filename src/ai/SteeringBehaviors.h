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

#include "Path.h"

class Kinematic;

class SteeringBehaviors
{
public:
    SteeringBehaviors(Kinematic* owner = nullptr);
    ~SteeringBehaviors();

    Vector2f compute(float dt);

    void setTarget(const Vector2f& target);
    const Path& getPath() const;
    void setPath(Path path);
    void setPanicDistance(float distance);
    void setArriveDistance(float distance);
    void setSeekDistance(float distance);

private:
    Kinematic* mOwner;
    Vector2f mTarget;
    Path mPath;
    float mPanicDistance;
    float mArriveDistance;
    float mSeekDistance;

    Vector2f seek(Vector2f target) const;
    Vector2f flee(Vector2f target) const;
    Vector2f arrive(Vector2f target) const;
    Vector2f followPath();

    inline Vector2f velocityToForce(const Vector2f& desiredVelocity, float dt) const;

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mOwner & mTarget & mPath & mPanicDistance & mArriveDistance & mSeekDistance;
    }
};
