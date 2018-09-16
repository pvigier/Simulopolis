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
 
#include "Road.h"

Road::Road(const std::string& name, Type type) :
    Tile(name, type, Category::ROAD)
{

}

Road::~Road()
{
    //dtor
}

std::unique_ptr<Tile> Road::clone() const
{
    return std::make_unique<Road>(mTextureName, mType);
}

void Road::updateVariant(const Tile* neighbors[3][3])
{
    sf::IntRect rect(0, 0, 132, 101);
    // Choose the right column
    switch (mType)
    {
        case Type::ROAD_SIDEWALK:
            rect.left = 132;
            break;
        case Type::BRIDGE:
            rect.left = 264;
            break;
        default:
            break;
    }
    // Choose the right row
    if (neighbors[0][1]->isRoad() && neighbors[1][0]->isRoad() &&
        neighbors[2][1]->isRoad() && neighbors[1][2]->isRoad())
        rect.top = 202;
    else if (neighbors[0][1]->isRoad() && neighbors[1][0]->isRoad() && neighbors[2][1]->isRoad())
        rect.top = 707;
    else if (neighbors[0][1]->isRoad() && neighbors[1][0]->isRoad() && neighbors[1][2]->isRoad())
        rect.top = 909;
    else if (neighbors[0][1]->isRoad() && neighbors[2][1]->isRoad() && neighbors[1][2]->isRoad())
        rect.top = 808;
    else if (neighbors[1][0]->isRoad() && neighbors[2][1]->isRoad() && neighbors[1][2]->isRoad())
        rect.top = 1010;
    else if (neighbors[0][1]->isRoad() && neighbors[1][0]->isRoad())
        rect.top = 505;
    else if (neighbors[1][0]->isRoad() && neighbors[2][1]->isRoad())
        rect.top = 404;
    else if (neighbors[2][1]->isRoad() && neighbors[1][2]->isRoad())
        rect.top = 606;
    else if (neighbors[1][2]->isRoad() && neighbors[0][1]->isRoad())
        rect.top = 303;
    else if ((neighbors[1][0]->isRoad() || neighbors[1][2]->isRoad()))
        rect.top = 101;
    else
        rect.top = 0;

    mSprite.setTextureRect(rect);
}
