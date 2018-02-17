#include "gui/GuiWidget.h"

GuiWidget::GuiWidget() : mVisible(true)
{
    //ctor
}

GuiWidget::~GuiWidget()
{
    //dtor
}

void GuiWidget::update()
{

}

void GuiWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mVisible)
        render(target, states);
}

bool GuiWidget::isVisible() const
{
    return mVisible;
}

void GuiWidget::setVisible(bool visible)
{
    mVisible = visible;
}

void GuiWidget::hover(sf::Vector2f position)
{

}

void GuiWidget::click(sf::Vector2f position)
{

}
