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

// Boost
#include <boost/serialization/vector.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>
// Tiles
#include "city/Road.h"
#include "city/CallForBids.h"
#include "city/Housing.h"
#include "city/Business.h"
#include "city/Industry.h"
#include "city/Service.h"
// Evaluators
#include "ai/GoalEnterCityEvaluator.h"
#include "ai/GoalGetBetterHomeEvaluator.h"
#include "ai/GoalGetBetterWorkEvaluator.h"
#include "ai/GoalLeaveCityEvaluator.h"
#include "ai/GoalRestEvaluator.h"
#include "ai/GoalShopEvaluator.h"
#include "ai/GoalWorkEvaluator.h"
// Goals
#include "ai/GoalEnterCity.h"
#include "ai/GoalGetBetterHome.h"
#include "ai/GoalGetBetterWork.h"
#include "ai/GoalLeaveCity.h"
#include "ai/GoalMoveTo.h"
#include "ai/GoalRest.h"
#include "ai/GoalShop.h"
#include "ai/GoalThink.h"
#include "ai/GoalWait.h"
#include "ai/GoalWork.h"
// Serialize
#include "serialize/serialize_message.h"

template<typename Archive>
void register_types(Archive& ar)
{
    // Tiles
    ar.template register_type<Road>();
    ar.template register_type<CallForBids>();
    ar.template register_type<Building>();
    ar.template register_type<Housing>();
    ar.template register_type<Industry>();
    ar.template register_type<Business>();
    ar.template register_type<Service>();
    // Markets
    ar.template register_type<Market<Good>>();
    ar.template register_type<Market<Good>::Event>();
    ar.template register_type<Market<Lease>>();
    ar.template register_type<Market<Lease>::Event>();
    ar.template register_type<Market<Work>>();
    ar.template register_type<Market<Work>::Event>();
    // Evaluators
    ar.template register_type<GoalEnterCityEvaluator>();
    ar.template register_type<GoalGetBetterHomeEvaluator>();
    ar.template register_type<GoalGetBetterWorkEvaluator>();
    ar.template register_type<GoalLeaveCityEvaluator>();
    ar.template register_type<GoalRestEvaluator>();
    ar.template register_type<GoalShopEvaluator>();
    ar.template register_type<GoalWorkEvaluator>();
    // Goals
    ar.template register_type<GoalEnterCity>();
    ar.template register_type<GoalGetBetterHome>();
    ar.template register_type<GoalGetBetterWork>();
    ar.template register_type<GoalLeaveCity>();
    ar.template register_type<GoalMoveTo>();
    ar.template register_type<GoalRest>();
    ar.template register_type<GoalShop>();
    ar.template register_type<GoalThink>();
    ar.template register_type<GoalWait>();
    ar.template register_type<GoalWork>();
}

void save_city(City& city);
void load_city(City& city);
