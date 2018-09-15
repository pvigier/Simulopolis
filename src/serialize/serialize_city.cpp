#include "serialize/serialize_city.h"
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "util/debug.h"

void save_city(City& city)
{
    std::ofstream file("saves/test.city");
    if (file)
    {
        boost::archive::text_oarchive oa(file);
        register_types(oa);
        oa << city;
        file.close();
    }
    else
        DEBUG("Fail to save the city");
}

void load_city(City& city)
{
    std::ifstream file("saves/test.city");
    if (file)
    {
        boost::archive::text_iarchive ia(file);
        register_types(ia);
        ia >> city;
        file.close();
    }
    else
        DEBUG("Fail to load the city");
}
