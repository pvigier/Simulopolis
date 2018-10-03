/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gui/GuiWidget.h"
#include "gui/GuiLayout.h"
#include "resource/PropertyList.h"
#include "resource/XmlDocument.h"

GuiWidget::GuiWidget(const XmlDocument* style) :
    mRoot(false), mParent(nullptr), mSizePolicies{SizePolicy::FIT_TO_CONTENT, SizePolicy::FIT_TO_CONTENT},
    mVisible(true), mStyle(style), mDirty(true)
{
    applyStyle();
}

GuiWidget::GuiWidget(const PropertyList& properties) : mRoot(false), mParent(nullptr),
    mSizePolicies{SizePolicy::FIT_TO_CONTENT, SizePolicy::FIT_TO_CONTENT}, mDirty(true)
{
    // Visibility
    mVisible = properties.get<bool>("visible", true);
    // Position
    setOutsidePosition(properties.get<sf::Vector2f>("position", sf::Vector2f()));
    // Size
    if (properties.has("size"))
        setFixedInsideSize(properties.get<sf::Vector2f>("size"));
    else
    {
        if (properties.has("width"))
            setFixedInsideWidth(properties.get<float>("width"));
        if (properties.has("height"))
            setFixedInsideHeight(properties.get<float>("height"));
    }
    // Style
    mStyle = properties.get<const XmlDocument*>("style", nullptr);
    applyStyle();
}

GuiWidget::~GuiWidget()
{

}

void GuiWidget::render(sf::RenderTarget& target, sf::RenderStates states, const sf::FloatRect& viewport) const
{
    // Culling
    if (mVisible && getOutsideRect().intersects(viewport))
    {
        draw(target, states);
        for (const GuiWidget* widget : mChildren)
            widget->render(target, states, viewport);
    }
}

void GuiWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mBackground, states);
}

void GuiWidget::setUp()
{

}

void GuiWidget::update()
{
    updateSize();
    updateAlignment();
    updateDesign();
    resetDirty();
}

void GuiWidget::tearDown()
{

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

GuiWidget* GuiWidget::remove(std::size_t i)
{
    GuiWidget* widget = *mChildren.erase(mChildren.begin() + i);
    setDirty();
    return widget;
}

void GuiWidget::remove(GuiWidget* widget)
{
    mChildren.erase(std::find(mChildren.begin(), mChildren.end(), widget));
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

bool GuiWidget::isRoot() const
{
    return mRoot;
}

void GuiWidget::setRoot(bool root)
{
    mRoot = root;
}

GuiWidget* GuiWidget::getParent()
{
    return mParent;
}

const GuiWidget* GuiWidget::getParent() const
{
    return mParent;
}

void GuiWidget::setParent(GuiWidget* parent)
{
    mParent = parent;
}

void GuiWidget::setGui(Gui* gui)
{
    mGui = gui;
}

const std::string& GuiWidget::getName() const
{
    return mName;
}

void GuiWidget::setName(const std::string& name)
{
    mName = name;
}

void GuiWidget::setLayout(std::unique_ptr<GuiLayout> layout)
{
    mLayout = std::move(layout);
    mLayout->setOwner(this);
    setDirty();
}

bool GuiWidget::isDirty() const
{
    return mDirty;
}

sf::Vector2f GuiWidget::getOutsidePosition() const
{
    return mOutsidePosition;
}

void GuiWidget::setOutsidePosition(sf::Vector2f position)
{
    if (position != mOutsidePosition)
    {
        mOutsidePosition = position;
        onOutsidePositionChanged();
        setDirty();
    }
}

sf::Vector2f GuiWidget::getInsidePosition() const
{
    return mInsidePosition;
}

sf::Vector2f GuiWidget::getOutsideSize() const
{
    return mOutsideSize;
}

sf::Vector2f GuiWidget::getInsideSize() const
{
    return mInsideSize;
}

void GuiWidget::setFixedInsideWidth(float width)
{
    if (!mSizePolicies[0] == SizePolicy::FIXED || width != mInsideSize.x)
    {
        mInsideSize.x = width;
        mSizePolicies[0] = SizePolicy::FIXED;
        onInsideWidthFixed();
        setDirty();
    }
}

void GuiWidget::setFixedInsideHeight(float height)
{
    if (!mSizePolicies[1] == SizePolicy::FIXED || height != mInsideSize.y)
    {
        mInsideSize.y = height;
        mSizePolicies[1] = SizePolicy::FIXED;
        onInsideHeightFixed();
        setDirty();
    }
}

void GuiWidget::setFixedInsideSize(sf::Vector2f size)
{
    setFixedInsideWidth(size.x);
    setFixedInsideHeight(size.y);
}

void GuiWidget::fitInsideWidthToContent()
{
    mSizePolicies[0] = SizePolicy::FIT_TO_CONTENT;
    setDirty();
}

void GuiWidget::fitInsideHeightToContent()
{
    mSizePolicies[1] = SizePolicy::FIT_TO_CONTENT;
    setDirty();
}

void GuiWidget::fitInsideSizeToContent()
{
    fitInsideWidthToContent();
    fitInsideHeightToContent();
}

sf::Vector2f GuiWidget::getContentSize() const
{
    return mLayout->computeSize();
}

sf::FloatRect GuiWidget::getOutsideRect() const
{
    return sf::FloatRect(mOutsidePosition, mOutsideSize);
}

void GuiWidget::updateSize()
{
    for (GuiWidget* widget : mChildren)
        widget->updateSize();
    if (mSizePolicies[0] == SizePolicy::FIT_TO_CONTENT || mSizePolicies[1] == SizePolicy::FIT_TO_CONTENT)
    {
        sf::Vector2f contentSize = getContentSize();
        if (mSizePolicies[0] == SizePolicy::FIT_TO_CONTENT && contentSize.x != mInsideSize.x)
            onContentWidthChanged(contentSize.x);
        if (mSizePolicies[1] == SizePolicy::FIT_TO_CONTENT && contentSize.y != mInsideSize.y)
            onContentHeightChanged(contentSize.y);
    }
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

bool GuiWidget::updateMouseWheelScrolled(float delta, bool processed)
{
    if (mVisible)
    {
        for (GuiWidget* widget : mChildren)
            processed = widget->updateMouseWheelScrolled(delta, processed) || processed;
        processed = onMouseWheelScroll(delta, processed) || processed;
    }
    return processed;
}

bool GuiWidget::updateKeyPressed(sf::Keyboard::Key key, bool processed)
{
    if (mVisible)
    {
        for (GuiWidget* widget : mChildren)
            processed = widget->updateKeyPressed(key, processed) || processed;
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
    if (mParent && !mParent->isDirty())
        mParent->setDirty();
}

void GuiWidget::resetDirty()
{
    mDirty = false;
    for (GuiWidget* widget : mChildren)
        widget->resetDirty();
}

void GuiWidget::onOutsidePositionChanged()
{
    mInsidePosition = mOutsidePosition;
}

void GuiWidget::onContentWidthChanged(float contentWidth)
{
    mOutsideSize.x = contentWidth;
    mInsideSize.x = contentWidth;
}

void GuiWidget::onContentHeightChanged(float contentHeight)
{
    mOutsideSize.y = contentHeight;
    mInsideSize.y = contentHeight;
}

void GuiWidget::onInsideWidthFixed()
{
    mOutsideSize.x = mInsideSize.x;
}

void GuiWidget::onInsideHeightFixed()
{
    mOutsideSize.y = mInsideSize.y;
}

void GuiWidget::onViewportSizeChanged(sf::Vector2u viewportSize)
{

}

bool GuiWidget::onHover(sf::Vector2f /*position*/, bool /*processed*/)
{
    return false;
}

bool GuiWidget::onPress(sf::Vector2f /*position*/, bool /*processed*/)
{
    return false;
}

bool GuiWidget::onRelease(sf::Vector2f /*position*/, bool /*processed*/)
{
    return false;
}

bool GuiWidget::onMouseWheelScroll(float /*delta*/, bool /*processed*/)
{
    return false;
}

bool GuiWidget::onKey(sf::Keyboard::Key /*key*/, bool /*processed*/)
{
    return false;
}

bool GuiWidget::onText(sf::Uint32 /*unicode*/, bool /*processed*/)
{
    return false;
}

void GuiWidget::updateAlignment()
{
    if (mLayout)
        mLayout->align();
    for (GuiWidget* widget : mChildren)
        widget->updateAlignment();
}

void GuiWidget::updateDesign()
{
    applyDesign();
    for (GuiWidget* widget : mChildren)
        widget->updateDesign();
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

void GuiWidget::applyDesign()
{
    mBackground.setPosition(mOutsidePosition);
    mBackground.setSize(mOutsideSize);
}
