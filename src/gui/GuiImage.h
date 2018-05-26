#pragma once

#include "SFML/Graphics.hpp"
#include "gui/GuiWidget.h"

class PropertyList;

class GuiImage : public GuiWidget
{
public:
    GuiImage(const sf::Sprite& sprite);
    GuiImage(const PropertyList& properties);
    virtual ~GuiImage();

    virtual void setPosition(sf::Vector2f position) override;
    virtual void setSize(sf::Vector2f size) override;
    void setSprite(sf::Sprite sprite);

protected:
    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Sprite mSprite;
};
