#pragma once

#include "SFML/Graphics.hpp"
#include "gui/GuiText.h"

class GuiInput : public GuiText
{
public:
    GuiInput(unsigned int characterSize, const XmlDocument* style);
    GuiInput(const PropertyList& properties);
    virtual ~GuiInput();

    virtual void setPosition(sf::Vector2f position) override;

protected:
    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual bool onText(sf::Uint32 unicode, bool processed) override;

private:
    std::size_t mCursor;
};
