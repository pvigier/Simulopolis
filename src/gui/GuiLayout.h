#pragma once

#include <SFML/Graphics.hpp>

class GuiWidget;
class PropertyList;

class GuiLayout
{
public:
    enum class HAlignment{Left, Center, Right};
    enum class VAlignment{Top, Center, Bottom};

    GuiLayout();
    GuiLayout(const PropertyList& properties);
    virtual ~GuiLayout();

    virtual void align() = 0;
    virtual sf::Vector2f computeSize() const = 0;

    void setOwner(GuiWidget* owner);

protected:
    GuiWidget* mOwner;
};
