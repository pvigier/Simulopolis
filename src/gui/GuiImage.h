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

    void setSprite(sf::Sprite sprite);

protected:
    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual void onPositionChanged() override;
    virtual void onSizeFixed() override;

private:
    sf::Sprite mSprite;
};
