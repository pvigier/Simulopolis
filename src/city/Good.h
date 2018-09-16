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

#include <string>
#include <boost/serialization/access.hpp>

class Building;
class Industry;

class Good
{
public:
    enum class Type : int {NECESSARY = 0, NORMAL, LUXURY};

    static std::string typeToString(Good::Type type);
    static float typeToHappiness(Good::Type type);

    Good(Type type, Industry* productionPlace);

    Type getType() const;
    const Building* getProductionPlace() const;

private:
    Type mType;
    Industry* mProductionPlace;

    // Serialization
    friend class boost::serialization::access;

    Good() = default;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mType & mProductionPlace;
    }
};

