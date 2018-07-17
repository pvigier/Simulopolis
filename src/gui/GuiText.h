#pragma once

#include "SFML/Graphics.hpp"
#include "gui/GuiWidget.h"

class XmlDocument;
class PropertyList;

class GuiText : public GuiWidget
{
public:
    GuiText(const sf::String& text, unsigned int characterSize, const XmlDocument* style);
    GuiText(const PropertyList& properties);
    virtual ~GuiText();

    virtual void setPosition(sf::Vector2f position) override;
    virtual void setCharacterSize(unsigned int characterSize);
    void setText(const sf::String& text);
    void setColor(sf::Color color);

protected:
    sf::Text mText;

    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void computeSize();
};
