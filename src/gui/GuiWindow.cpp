#include "gui/GuiWindow.h"
#include "resource/ResourceManager.h"

GuiWindow::GuiWindow(sf::Vector2f size, const XmlDocument* style) :
    mStyle(style)
{
    setSize(size);
    applyStyle();
}

GuiWindow::GuiWindow(const PropertyList& properties) :
    GuiWidget(properties), mStyle(properties.get<const XmlDocument*>("style"))
{
    setPosition(mPosition);
    setSize(mSize);
    applyStyle();
}

void GuiWindow::setPosition(sf::Vector2f position)
{
    GuiWidget::setPosition(position);
    mBody.setPosition(position);
    mBar.setPosition(position);
}

void GuiWindow::setSize(sf::Vector2f size)
{
    GuiWidget::setSize(size);
    mBody.setSize(size);
    mBar.setSize(sf::Vector2f(size.x, mStyle->getFirstChildByName("bar").getAttributes().get<float>("height")));
}

void GuiWindow::onHover(sf::Vector2f position)
{

}

void GuiWindow::onPress(sf::Vector2f position)
{

}

void GuiWindow::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mBody);
    target.draw(mBar);
}

void GuiWindow::applyStyle()
{
    // Body
    mBody.setOutlineThickness(-mStyle->getFirstChildByName("border").getAttributes().get<int>("size"));
    mBody.setFillColor(mStyle->getFirstChildByName("body").getAttributes().get<sf::Color>("color"));
    mBody.setOutlineColor(mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("color"));
    // Bar
    mBar.setOrigin(0.0f, mStyle->getFirstChildByName("bar").getAttributes().get<float>("height") -
        mStyle->getFirstChildByName("border").getAttributes().get<int>("size"));
    mBar.setOutlineThickness(-mStyle->getFirstChildByName("border").getAttributes().get<int>("size"));
    mBar.setFillColor(mStyle->getFirstChildByName("bar").getAttributes().get<sf::Color>("color"));
    mBar.setOutlineColor(mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("color"));
}
