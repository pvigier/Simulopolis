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

GuiImage::~GuiImage()
{
    //dtor
}


void GuiImage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    GuiWidget::draw(target, states);
    target.draw(mSprite, states);
}

void GuiImage::setSprite(sf::Sprite sprite)
{
    mSprite = sprite;
    setDirty();
}

void GuiImage::onOutsidePositionChanged()
{
    GuiWidget::onOutsidePositionChanged();
    mSprite.setPosition(mInsidePosition);
}

void GuiImage::onInsideSizeFixed()
{
    GuiWidget::onInsideSizeFixed();
    if (mSprite.getTexture())
    {
        sf::Vector2f textureSize(mSprite.getTexture()->getSize());
        mSprite.setScale(mInsideSize.x / textureSize.x, mInsideSize.y / textureSize.y);
    }
}
