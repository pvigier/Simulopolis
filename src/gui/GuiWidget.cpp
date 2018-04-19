#include "gui/GuiWidget.h"
#include "gui/GuiLayout.h"
#include "resource/PropertyList.h"

GuiWidget::GuiWidget() : mRoot(false), mParent(nullptr), mVisible(true), mDirty(true)
{
    //ctor
}

GuiWidget::GuiWidget(const PropertyList& properties) : mRoot(false), mParent(nullptr), mDirty(true)
{
    mVisible = properties.get<bool>("visible", true);
    mPosition = properties.get<sf::Vector2f>("position", sf::Vector2f());
    mSize = properties.get<sf::Vector2f>("size", sf::Vector2f());
}

GuiWidget::~GuiWidget()
{

}

void GuiWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mVisible)
    {
        render(target, states);
        for (const GuiWidget* widget : mChildren)
            target.draw(*widget);
    }
}

void GuiWidget::update()
{
    if (mLayout != nullptr)
    {
        mLayout->align();
        for (GuiWidget* widget : mChildren)
            widget->update();
    }
    mDirty = false;
}

void GuiWidget::add(GuiWidget* widget)
{
    widget->setParent(this);
    mChildren.push_back(widget);
    setDirty();
}

std::vector<GuiWidget*>& GuiWidget::getChildren()
{
    return mChildren;
}

const std::vector<GuiWidget*>& GuiWidget::getChildren() const
{
    return mChildren;
}

void GuiWidget::fitSizeToContent()
{
    setSize(mLayout->computeSize());
}

const std::string& GuiWidget::getName() const
{
    return mName;
}

void GuiWidget::setName(const std::string& name)
{
    mName = name;
}

bool GuiWidget::isRoot() const
{
    return mRoot;
}

void GuiWidget::setRoot(bool root)
{
    mRoot = root;
}

void GuiWidget::setParent(GuiWidget* parent)
{
    mParent = parent;
}

void GuiWidget::setLayout(std::unique_ptr<GuiLayout> layout)
{
    mLayout = std::move(layout);
    mLayout->setOwner(this);
    setDirty();
}

sf::Vector2f GuiWidget::getPosition() const
{
    return mPosition;
}

void GuiWidget::setPosition(sf::Vector2f position)
{
    mPosition = position;
    setDirty();
}

sf::Vector2f GuiWidget::getSize() const
{
    return mSize;
}

void GuiWidget::setSize(sf::Vector2f size)
{
    mSize = size;
    setDirty();
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

bool GuiWidget::isDirty() const
{
    return mDirty;
}

void GuiWidget::updateMouseMoved(sf::Vector2f position)
{
    if (mVisible)
    {
        onHover(position);
        for (GuiWidget* widget : mChildren)
            widget->updateMouseMoved(position);
    }
}

void GuiWidget::updateMouseButtonPressed(sf::Vector2f position)
{
    if (mVisible)
    {
        onPress(position);
        for (GuiWidget* widget : mChildren)
            widget->updateMouseButtonPressed(position);
    }
}

void GuiWidget::updateMouseButtonReleased(sf::Vector2f position)
{
    if (mVisible)
    {
        onRelease(position);
        for (GuiWidget* widget : mChildren)
            widget->updateMouseButtonReleased(position);
    }
}

bool GuiWidget::hasGuiEvents() const
{
    return false;
}

void GuiWidget::setDirty()
{
    mDirty = true;
    if (mParent != nullptr)
        mParent->setDirty();
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
