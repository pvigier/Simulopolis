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

#include "GoalThink.h"
#include "city/Person.h"

GoalThink::GoalThink(Person* owner) : Goal(owner)
{
    //ctor
}

void GoalThink::activate()
{
    mState = State::ACTIVE;
    // Add subgoals
    clearSubgoals();
    arbitrate();
}

Goal::State GoalThink::process()
{
    activateIfInactive();

    mState = processSubgoals();
    if (mState == State::COMPLETED || mState == State::FAILED)
        //mState = State::INACTIVE;
        activate();

    return mState;
}

void GoalThink::terminate()
{

}

void GoalThink::addEvaluator(std::unique_ptr<GoalEvaluator>&& evaluator)
{
    mEvaluators.emplace_back(std::move(evaluator));
}

std::vector<std::unique_ptr<GoalEvaluator>>& GoalThink::getEvaluators()
{
    return mEvaluators;
}

bool GoalThink::handle(Message message)
{
    return forward(message);
}

void GoalThink::arbitrate()
{
    float maxDesirability = std::numeric_limits<float>::lowest();
    GoalEvaluator* bestEvaluator = nullptr;
    for (std::unique_ptr<GoalEvaluator>& evaluator : mEvaluators)
    {
        float desirability = evaluator->computeDesirability(mOwner);
        if (desirability > maxDesirability)
        {
            maxDesirability = desirability;
            bestEvaluator = evaluator.get();
        }
    }

    if (maxDesirability > 0.0f && bestEvaluator)
        bestEvaluator->setGoal(mOwner);
}

std::string GoalThink::toString() const
{
    if (mSubgoals.empty())
        return "Nothing";
    else
        return mSubgoals.front()->toString();
}
