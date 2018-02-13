#pragma once

#include "gui/GuiWidget.h"

class GuiLayout : public GuiWidget
{
public:
    enum class HAlignment{Left, Center, Right};
    enum class VAlignment {Top, Center, Bottom};

    GuiLayout();
    virtual ~GuiLayout();

    virtual sf::Vector2f getPosition() const override;
    virtual void setPosition(sf::Vector2f position) override;
    virtual sf::Vector2f getSize() const override;
    virtual void setSize(sf::Vector2f size) override;

protected:
    sf::Vector2f mPosition;
    sf::Vector2f mSize;

    virtual void align() = 0;
};
