#pragma once

#include <string>
#include "gui/GuiWidget.h"

class XmlDocument;

class GuiButton : public GuiWidget
{
public:
    enum class State{NORMAL, HOVERED, PRESSED, FORCE_PRESSED, DISABLED};

    GuiButton(sf::Vector2f size);
    GuiButton(const PropertyList& properties);

    void setState(State state);

    bool hitButton(sf::Vector2f position) const;

protected:
    virtual bool onHover(sf::Vector2f position, bool processed) override;
    virtual bool onPress(sf::Vector2f position, bool processed) override;
    virtual bool onRelease(sf::Vector2f position, bool processed) override;

private:
    State mState;
};
