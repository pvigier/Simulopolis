#pragma once

#include <SFML/Graphics.hpp>

class GuiWidget;
class PropertyList;

class GuiLayout
{
public:
    enum class HAlignment{Left, Center, Right};
    enum class VAlignment{Top, Center, Bottom};

    struct Margins
    {
        float left;
        float top;
        float right;
        float bottom;
    };

    GuiLayout(Margins margins = Margins{0.0f, 0.0f, 0.0f, 0.0f});
    GuiLayout(const PropertyList& properties);
    virtual ~GuiLayout();

    virtual void align() = 0;
    virtual sf::Vector2f computeSize() const = 0;

    void setOwner(GuiWidget* owner);
    void setMargins(Margins margins);

protected:
    GuiWidget* mOwner;
    Margins mMargins;
};
