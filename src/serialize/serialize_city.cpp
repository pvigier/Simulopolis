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

#include "serialize/serialize_city.h"
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "util/debug.h"

void save_city(City& city, const std::string& path)
{
    DEBUG("Save " << city.getName() << " at " << path << "\n");
    std::ofstream file(path);
    if (file)
    {
        boost::archive::text_oarchive oa(file);
        register_types(oa);
        oa << city;
        file.close();
    }
    else
        DEBUG("Fail to save the city\n");
}

void load_city(City& city, const std::string& path)
{
    DEBUG("Load " << city.getName() << " from " << path << "\n");
    std::ifstream file(path);
    if (file)
    {
        boost::archive::text_iarchive ia(file);
        register_types(ia);
        ia >> city;
        file.close();
    }
    else
        DEBUG("Fail to load the city\n");
}
