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
 
#include "ai/GoalGetBetterWorkEvaluator.h"
#include "util/common.h"
#include "city/Person.h"
#include "ai/GoalGetBetterWork.h"

GoalGetBetterWorkEvaluator::GoalGetBetterWorkEvaluator(float bias) : GoalEvaluator(bias)
{
    //ctor
}

float GoalGetBetterWorkEvaluator::computeDesirability(Person* person)
{
    if (person->getHome())
        return mBias * sigmoid(-person->getLastMonthOutcome());
    return 0.0f;
}

void GoalGetBetterWorkEvaluator::setGoal(Person* person)
{
    person->getLongTermBrain().pushFront(std::make_unique<GoalGetBetterWork>(person, 3));
}
