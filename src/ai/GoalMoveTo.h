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

#include <boost/serialization/base_object.hpp>
#include "ai/Goal.h"
#include "ai/Path.h"

class Tile;

class GoalMoveTo : public Goal
{
public:
    GoalMoveTo(Person* owner, const Tile* target);

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual std::string toString() const override;

private:
    static constexpr float ARRIVE_DISTANCE = 1.0f;
    static constexpr float MIN_DISTANCE = 256.0f;

    const Tile* mTarget;

    // Serialization
    friend class boost::serialization::access;

    GoalMoveTo() = default;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<Goal>(*this);
        ar & mTarget;
    }
};
