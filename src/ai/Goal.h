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

#include <deque>
#include <boost/serialization/access.hpp>
#include "util/NonCopyable.h"
#include "util/NonMovable.h"
#include "message/Message.h"

class Person;

class Goal : public NonCopyable, public NonMovable
{
public:
    enum class State{ACTIVE, INACTIVE, COMPLETED, FAILED};

    Goal(Person* owner);
    virtual ~Goal();

    virtual void activate() = 0;
    virtual State process() = 0;
    virtual void terminate() = 0;

    virtual bool handle(Message message);

    State getState() const;
    bool isActive() const;
    bool isInactive() const;
    bool isCompleted() const;
    bool hasFailed() const;

    void pushFront(std::unique_ptr<Goal>&& goal);
    void pushBack(std::unique_ptr<Goal>&& goal);
    void clearSubgoals();
    bool hasSubgoals() const;

    virtual std::string toString() const;

protected:
    Person* mOwner;
    State mState;
    std::deque<std::unique_ptr<Goal>> mSubgoals;

    Goal() = default; // Only for serialization

    void activateIfInactive();
    void reactivateIfFailed();

    State processSubgoals();
    bool forward(Message message);

private:
    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mOwner & mState & mSubgoals;
    }
};
