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

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setSprite(sf::Sprite sprite);

protected:
    virtual void onOutsidePositionChanged() override;
    virtual void onInsideSizeFixed() override;

private:
    sf::Sprite mSprite;
};
