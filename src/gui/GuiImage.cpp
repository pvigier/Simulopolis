#include "gui/GuiImage.h"
#include "resource/PropertyList.h"

GuiImage::GuiImage(const sf::Texture& texture) : mSprite(texture)
{
    GuiWidget::setSize(sf::Vector2f(texture.getSize()));
}

GuiImage::GuiImage(const PropertyList& properties) : GuiWidget(properties)
{
    if (properties.has("texture"))
    {
        mSprite.setTexture(properties.get<const sf::Texture&>("texture"));
        GuiWidget::setSize(sf::Vector2f(mSprite.getTextureRect().width, mSprite.getTextureRect().height));
    }
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
