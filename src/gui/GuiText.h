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

    void setCharacterSize(unsigned int characterSize);
    const sf::Text& getText() const;
    const sf::String& getString() const;
    void setString(const sf::String& text);
    void setColor(sf::Color color);

protected:
    sf::Text mText;

    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual void onOutsidePositionChanged() override;

private:
    sf::Vector2f computeSize() const;
};
