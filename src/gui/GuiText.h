#pragma once

#include "SFML/Graphics.hpp"
#include "gui/GuiWidget.h"
#include "gui/GuiStyle.h"

class PropertyList;

class GuiText : public GuiWidget
{
public:
    GuiText(const std::string& text, unsigned int characterSize, const GuiStyle& style);
    GuiText(const PropertyList& properties);
    virtual ~GuiText();

    virtual void setPosition(sf::Vector2f position) override;
    virtual void setCharacterSize(unsigned int characterSize);
    void setText(const std::string& text);

protected:
    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Text mText;

    void computeSize();
};
