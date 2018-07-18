#include "gui/GuiWidget.h"
#include "gui/GuiLayout.h"
#include "resource/PropertyList.h"
#include "resource/XmlDocument.h"

GuiWidget::GuiWidget(const XmlDocument* style) :
    mRoot(false), mParent(nullptr), mVisible(true), mFixedSize(false), mStyle(style), mDirty(true)
{
    applyStyle();
}

GuiWidget::GuiWidget(const PropertyList& properties) : mRoot(false), mParent(nullptr), mDirty(true)
{
    mVisible = properties.get<bool>("visible", true);
    mPosition = properties.get<sf::Vector2f>("position", sf::Vector2f());
    mSize = properties.get<sf::Vector2f>("size", sf::Vector2f());
    mFixedSize = properties.has("size");
    setPosition(mPosition);
    setSize(mSize);
    mStyle = properties.get<const XmlDocument*>("style", nullptr);
    applyStyle();
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
    updateSize();
    updateAlignment();
    mDirty = false;
}

void GuiWidget::add(GuiWidget* widget)
{
    widget->setParent(this);
    mChildren.push_back(widget);
    setDirty();
}

void GuiWidget::insert(std::size_t i, GuiWidget* widget)
{
    widget->setParent(this);
    mChildren.insert(mChildren.begin() + i, widget);
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
    mFixedSize = false;
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
    mBackground.setPosition(position);
    setDirty();
}

sf::Vector2f GuiWidget::getSize() const
{
    return mSize;
}

void GuiWidget::setFixedSize(sf::Vector2f size)
{
    setSize(size);
    mFixedSize = true;
}

sf::FloatRect GuiWidget::getRect() const
{
    return sf::FloatRect(mPosition, mSize);
}

void GuiWidget::setBackgroundColor(const sf::Color& color)
{
    mBackground.setFillColor(color);
}

void GuiWidget::setBorderSize(int borderSize)
{
    mBackground.setOutlineThickness(borderSize);
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

bool GuiWidget::updateMouseMoved(sf::Vector2f position, bool processed)
{
    if (mVisible)
    {
        for (GuiWidget* widget : mChildren)
            processed = widget->updateMouseMoved(position, processed) || processed;
        processed = onHover(position, processed) || processed;
    }
    return processed;
}

bool GuiWidget::updateMouseButtonPressed(sf::Vector2f position, bool processed)
{
    if (mVisible)
    {
        for (GuiWidget* widget : mChildren)
            processed = widget->updateMouseButtonPressed(position, processed) || processed;
        processed = onPress(position, processed) || processed;
    }
    return processed;
}

bool GuiWidget::updateMouseButtonReleased(sf::Vector2f position, bool processed)
{
    if (mVisible)
    {
        for (GuiWidget* widget : mChildren)
            processed = widget->updateMouseButtonReleased(position, processed) || processed;
        processed = onRelease(position, processed) || processed;
    }
    return processed;
}

bool GuiWidget::updateKeyReleased(sf::Keyboard::Key key, bool processed)
{
    if (mVisible)
    {
        for (GuiWidget* widget : mChildren)
            processed = widget->updateKeyReleased(key, processed) || processed;
        processed = onKey(key, processed) || processed;
    }
    return processed;
}

bool GuiWidget::updateTextEntered(sf::Uint32 unicode, bool processed)
{
    if (mVisible)
    {
        for (GuiWidget* widget : mChildren)
            processed = widget->updateTextEntered(unicode, processed) || processed;
        processed = onText(unicode, processed) || processed;
    }
    return processed;
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

void GuiWidget::setSize(sf::Vector2f size)
{
    mSize = size;
    mBackground.setSize(size);
    setDirty();
}

void GuiWidget::updateSize()
{
    for (GuiWidget* widget : mChildren)
        widget->updateSize();
    if (!mFixedSize)
        fitSizeToContent();
}

void GuiWidget::updateAlignment()
{
    if (mLayout != nullptr)
        mLayout->align();
    for (GuiWidget* widget : mChildren)
        widget->updateAlignment();
}

void GuiWidget::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mBackground);
}

bool GuiWidget::onHover(sf::Vector2f position, bool processed)
{
    return false;
}

bool GuiWidget::onPress(sf::Vector2f position, bool processed)
{
    return false;
}

bool GuiWidget::onRelease(sf::Vector2f position, bool processed)
{
    return false;
}

bool GuiWidget::onKey(sf::Keyboard::Key key, bool processed)
{
    return false;
}

bool GuiWidget::onText(sf::Uint32 unicode, bool processed)
{
    return false;
}

void GuiWidget::applyStyle()
{
    sf::Color backgroundColor = sf::Color::Transparent;
    sf::Color borderColor = sf::Color::Transparent;
    int borderSize = 0;
    if (mStyle)
    {
        if (mStyle->hasChildren("background"))
            backgroundColor = mStyle->getFirstChildByName("background").getAttributes().get<sf::Color>("color", backgroundColor);
        if (mStyle->hasChildren("border"))
        {
            borderColor = mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("color", borderColor);
            borderSize = mStyle->getFirstChildByName("border").getAttributes().get<int>("size", borderSize);
        }
    }
    mBackground.setFillColor(backgroundColor);
    mBackground.setOutlineColor(borderColor);
    mBackground.setOutlineThickness(borderSize);
}
