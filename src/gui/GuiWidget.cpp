#include "gui/GuiWidget.h"
#include "gui/GuiLayout.h"

GuiWidget::GuiWidget() : mVisible(true)
{
    //ctor
}

GuiWidget::~GuiWidget()
{

}

void GuiWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mVisible)
    {
        render(target, states);
        for (const GuiWidgetPtr& widget : mChildren)
            target.draw(*widget);
    }
}

void GuiWidget::update()
{
    if (mLayout != nullptr)
        mLayout->align();
    for (GuiWidgetPtr& widget : mChildren)
        widget->update();
}

void GuiWidget::add(GuiWidgetPtr widget)
{
    mChildren.push_back(widget);
    update();
}

std::vector<GuiWidgetPtr>& GuiWidget::getChildren()
{
    return mChildren;
}

const std::vector<GuiWidgetPtr>& GuiWidget::getChildren() const
{
    return mChildren;
}

void GuiWidget::fitSizeToContent()
{
    setSize(mLayout->computeSize());
}

void GuiWidget::setLayout(std::unique_ptr<GuiLayout> layout)
{
    mLayout = std::move(layout);
    mLayout->setOwner(this);
    update();
}

sf::Vector2f GuiWidget::getPosition() const
{
    return mPosition;
}

void GuiWidget::setPosition(sf::Vector2f position)
{
    mPosition = position;
    update();
}

sf::Vector2f GuiWidget::getSize() const
{
    return mSize;
}

void GuiWidget::setSize(sf::Vector2f size)
{
    mSize = size;
    update();
}

sf::FloatRect GuiWidget::getRect() const
{
    return sf::FloatRect(mPosition, mSize);
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
    {
        onHover(position);
        for (GuiWidgetPtr& widget : mChildren)
            widget->updateMouseMoved(position);
    }
}

void GuiWidget::updateMouseButtonPressed(sf::Vector2f position)
{
    if (mVisible)
    {
        onPress(position);
        for (GuiWidgetPtr& widget : mChildren)
            widget->updateMouseButtonPressed(position);
    }
}

void GuiWidget::updateMouseButtonReleased(sf::Vector2f position)
{
    if (mVisible)
    {
        onRelease(position);
        for (GuiWidgetPtr& widget : mChildren)
            widget->updateMouseButtonReleased(position);
    }
}

void GuiWidget::render(sf::RenderTarget& target, sf::RenderStates states) const
{

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
