#include "GuiImage.h"

GuiImage::GuiImage(const sf::Texture& texture) : mSprite(texture)
{
    //ctor
}

GuiImage::~GuiImage()
{
    //dtor
}

void GuiImage::setPosition(sf::Vector2f position)
{
    GuiWidget::setPosition(position);
    mSprite.setPosition(position);
}

void GuiImage::setSize(sf::Vector2f size)
{
    GuiWidget::setSize(size);
    sf::Vector2f textureSize(mSprite.getTexture()->getSize());
    mSprite.setScale(size.x / textureSize.x, size.y / textureSize.y);
}

void GuiImage::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite);
}
