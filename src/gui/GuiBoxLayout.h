#pragma once

#include <vector>
#include "gui/GuiLayout.h"

class GuiBoxLayout : public GuiLayout
{
public:
    GuiBoxLayout();
    virtual ~GuiBoxLayout();

    virtual void update() override;

    virtual sf::FloatRect getRect() const override;

    virtual void onHover(sf::Vector2f position) override;
    virtual void onPress(sf::Vector2f position) override;
    virtual void onRelease(sf::Vector2f position) override;

    virtual void add(GuiWidget* widget);
    virtual void setSpacing(float spacing);
    void setHAlignment(HAlignment alignment);
    void setVAlignment(VAlignment alignment);
    void fitSizeToContent();

protected:
    std::vector<GuiWidget*> mWidgets;
    float mSpacing;
    HAlignment mHAlignment;
    VAlignment mVAlignment;

    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual sf::Vector2f computeSize() const = 0;
};
