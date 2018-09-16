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

template <typename T> class Market;
class Lease;

class GoalGetBetterHome : public Goal
{
public:
    GoalGetBetterHome(Person* owner, unsigned int nbMonthsBeforeFailing);
    virtual ~GoalGetBetterHome();

    virtual void activate() override;
    virtual State process() override;
    virtual void terminate() override;

    virtual bool handle(Message message) override;

    virtual std::string toString() const override;

private:
    unsigned int mNbMonthsBeforeFailing;
    const Market<Lease>* mMarket;

    // Serialization
    friend class boost::serialization::access;

    GoalGetBetterHome() = default;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<Goal>(*this);
        ar & mNbMonthsBeforeFailing & mMarket;
    }
};
