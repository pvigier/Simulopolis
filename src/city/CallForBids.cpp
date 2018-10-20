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

#include "CallForBids.h"
#include "resource/TextureManager.h"

CallForBids::CallForBids(const std::string& name, Type type, const sf::Color& signColor) :
    Tile(name, type, Category::CALL_FOR_BIDS), mSignColor(signColor)
{
    setUpSign();
}

void CallForBids::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
    // Sign
    target.draw(mSignSprite, states);
}

std::unique_ptr<Tile> CallForBids::clone() const
{
    return std::make_unique<CallForBids>(mTextureName, mType, mSignColor);
}

void CallForBids::setPosition(sf::Vector2i coordinates, sf::Vector2f position)
{
    mCoordinates = coordinates;
    mSprite.setPosition(position);
    mSignSprite.setPosition(position + sf::Vector2f(53.0f, -8.0f));
}

sf::FloatRect CallForBids::getBounds() const
{
    sf::FloatRect bounds = mSprite.getGlobalBounds();
    bounds.top -= 8.0f;
    bounds.height += 8.0f;
    return bounds;
}

void CallForBids::setUpSign()
{
    mSignSprite.setTexture(sTextureManager->getTexture("sign"));
    mSignSprite.setColor(mSignColor);
}
