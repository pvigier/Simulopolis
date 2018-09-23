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

#include "city/Good.h"
#include "city/Industry.h"

std::string Good::typeToString(Good::Type type)
{
    switch (type)
    {
        case Good::Type::NECESSARY:
            return "Necessary";
        case Good::Type::NORMAL:
            return "Normal";
        case Good::Type::LUXURY:
            return "Luxury";
        default:
            return "";
    }
}

float Good::typeToHappiness(Good::Type type)
{
    switch (type)
    {
        case Good::Type::NECESSARY:
            return 0.05f;
        case Good::Type::NORMAL:
            return 0.1f;
        case Good::Type::LUXURY:
            return 0.2f;
        default:
            return 0.0f;
    }
}

Good::Good(Type type, Industry* productionPlace) : mType(type), mProductionPlace(productionPlace)
{

}

Good::Type Good::getType() const
{
    return mType;
}

const Building* Good::getProductionPlace() const
{
    return mProductionPlace;
}
