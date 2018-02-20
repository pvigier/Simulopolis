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

void GuiWidget::updateMouseMoved(sf::Vector2f position)
{
    if (mVisible)
        onHover(position);
}

void GuiWidget::updateMouseButtonPressed(sf::Vector2f position)
{
    if (mVisible)
        onPress(position);
}

void GuiWidget::updateMouseButtonReleased(sf::Vector2f position)
{
    if (mVisible)
        onRelease(position);
}

void GuiWidget::onHover(sf::Vector2f position)
{

}

void GuiWidget::onPress(sf::Vector2f position)
{

}

void GuiWidget::onRelease(sf::Vector2f position)
{

}
