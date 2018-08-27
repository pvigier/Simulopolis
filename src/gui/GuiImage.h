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

    virtual void onOutsidePositionChanged() override;
    virtual void onInsideSizeFixed() override;

private:
    sf::Sprite mSprite;
};
