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
 
#include "ai/GoalRestEvaluator.h"
#include "city/Person.h"
#include "ai/GoalRest.h"

GoalRestEvaluator::GoalRestEvaluator(float bias) : GoalEvaluator(bias)
{
    //ctor
}

float GoalRestEvaluator::computeDesirability(Person* person)
{
    if (person->getHome())
        return mBias * (1.0f - person->getNeed(Person::Need::ENERGY));
    return 0.0f;
}

void GoalRestEvaluator::setGoal(Person* person)
{
    person->getShortTermBrain().pushFront(std::make_unique<GoalRest>(person));
}
