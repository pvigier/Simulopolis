#include "gui/GuiImage.h"
#include "resource/PropertyList.h"

GuiImage::GuiImage(const sf::Sprite& sprite) : mSprite(sprite)
{
    GuiWidget::setSize(sf::Vector2f(sprite.getTextureRect().width, sprite.getTextureRect().height));
    mFixedSize = true;
}

GuiImage::GuiImage(const PropertyList& properties) : GuiWidget(properties)
{
    if (properties.has("texture"))
        mSprite.setTexture(properties.get<const sf::Texture&>("texture"));
    if (properties.has("position"))
        mSprite.setPosition(mPosition);
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
        GuiWidget::setSize(sf::Vector2f(mSprite.getTextureRect().width, mSprite.getTextureRect().height));
    mFixedSize = true;
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

void GuiImage::setSprite(sf::Sprite sprite)
{
    mSprite = sprite;
    setDirty();
}

void GuiImage::setSize(sf::Vector2f size)
{
    GuiWidget::setSize(size);
    sf::Vector2f textureSize(mSprite.getTexture()->getSize());
    mSprite.setScale(size.x / textureSize.x, size.y / textureSize.y);
}

void GuiImage::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}
