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

#include "gui/GuiImage.h"
#include "resource/PropertyList.h"

GuiImage::GuiImage(const sf::Sprite& sprite) : mSprite(sprite)
{
    setFixedInsideSize(sf::Vector2f(sprite.getTextureRect().width, sprite.getTextureRect().height));
}

GuiImage::GuiImage(const PropertyList& properties) : GuiWidget(properties)
{
    if (properties.has("texture"))
        mSprite.setTexture(properties.get<const sf::Texture&>("texture"));
    if (properties.has("rect"))
        mSprite.setTextureRect(properties.get<sf::IntRect>("rect"));
    if (properties.has("size"))
    {
        sf::Vector2f scale;
        scale.x = properties.get<sf::Vector2f>("size").x / mSprite.getTextureRect().width;
        scale.y = properties.get<sf::Vector2f>("size").y / mSprite.getTextureRect().height;
        mSprite.scale(scale);
    }
    else
        setFixedInsideSize(sf::Vector2f(mSprite.getTextureRect().width, mSprite.getTextureRect().height));
}

void GuiImage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    GuiWidget::draw(target, states);
    target.draw(mSprite, states);
}

void GuiImage::setSprite(sf::Sprite sprite)
{
    mSprite = sprite;
    applyDesign();
}

void GuiImage::applyDesign()
{
    mSprite.setPosition(mInsidePosition);
    if (mSprite.getTexture())
    {
        sf::Vector2f textureSize(mSprite.getTextureRect().width, mSprite.getTextureRect().height);
        mSprite.setScale(mInsideSize.x / textureSize.x, mInsideSize.y / textureSize.y);
    }
}
