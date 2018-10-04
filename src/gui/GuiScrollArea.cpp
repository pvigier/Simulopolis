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

#include "GuiScrollArea.h"
#include "resource/XmlDocument.h"
#include "util/common.h"

GuiScrollArea::GuiScrollArea(sf::Vector2i maxVisibleSize, const XmlDocument* style) :
    GuiWidget(style), mFocus(false), mScrollbarVisible(false), mOffset(0.0f),
    mMaxVisibleSizePolicies{SizePolicy::FIT_TO_CONTENT, SizePolicy::FIT_TO_CONTENT},
    mMaxVisibleSize(maxVisibleSize), mScrolling(false), mAnchor(0.0f)
{
    mRenderTexture.create(mMaxVisibleSize.x, mMaxVisibleSize.y);
}

GuiScrollArea::GuiScrollArea(const PropertyList& properties) :
    GuiWidget(properties), mFocus(false), mScrollbarVisible(false), mOffset(0.0f),
    mMaxVisibleSizePolicies{SizePolicy::FIT_TO_CONTENT, SizePolicy::FIT_TO_CONTENT},
    mScrolling(false), mAnchor(0.0f)
{
    if (properties.has("maxVisibleSize"))
    {
        if (properties.isPercentageVector("maxVisibleSize"))
        {
            mMaxVisibleSizeRatio = properties.getPercentageVector("maxVisibleSize");
            mMaxVisibleSizePolicies.fill(SizePolicy::RATIO);
        }
        else
        {
            mMaxVisibleSize = properties.get<sf::Vector2i>("maxVisibleSize");
            mRenderTexture.create(mMaxVisibleSize.x, mMaxVisibleSize.y);
        }
    }
}

void GuiScrollArea::render(sf::RenderTarget& target, sf::RenderStates states, const sf::FloatRect& viewport) const
{
    // Culling
    if (mVisible && getOutsideRect().intersects(viewport))
    {
        mRenderTexture.clear(sf::Color::Transparent);
        sf::FloatRect newViewport(mInsidePosition.x, mInsidePosition.y + mOffset, mInsideSize.x, mInsideSize.y);
        for (const GuiWidget* widget : mChildren)
            widget->render(mRenderTexture, states, newViewport);
        mRenderTexture.display();
        draw(target, states);
    }
}

void GuiScrollArea::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    GuiWidget::draw(target, states);
    target.draw(mSprite, states);
    // Scrollbar
    if (mScrollbarVisible)
    {
        target.draw(mLine.data(), 2, sf::Lines);
        target.draw(mScrollButton);
    }
}

void GuiScrollArea::setViewportSize(sf::Vector2u viewportSize)
{
    // Inside size
    if (mSizePolicies[0] == SizePolicy::RATIO)
    {
        onViewportWidthChanged(viewportSize.x);
        setDirty();
    }
    if (mSizePolicies[1] == SizePolicy::RATIO)
    {
        onViewportHeightChanged(viewportSize.y);
        setDirty();
    }
    // Tmp
    if (mMaxVisibleSizePolicies[0] == SizePolicy::RATIO || mMaxVisibleSizePolicies[1] == SizePolicy::RATIO)
    {
        if (mMaxVisibleSizePolicies[0] == SizePolicy::RATIO)
        {
            mMaxVisibleSize.x = viewportSize.x * mMaxVisibleSizeRatio.x;
            setDirty();
        }
        if (mMaxVisibleSizePolicies[1] == SizePolicy::RATIO)
        {
            mMaxVisibleSize.y = viewportSize.y * mMaxVisibleSizeRatio.y;
            setDirty();
        }
        mRenderTexture.create(mMaxVisibleSize.x, mMaxVisibleSize.y);
    }
    for (GuiWidget* widget : mChildren)
        widget->setViewportSize(viewportSize);
}

bool GuiScrollArea::updateMouseMoved(sf::Vector2f position, bool processed)
{
    if (mVisible)
    {
        mFocus = mBackground.getGlobalBounds().contains(position);
        sf::Vector2f offsetPosition = sf::Vector2f(position.x, position.y + mOffset);
        if (!mFocus)
            offsetPosition = sf::Vector2f(-1.0f, 1.0f);
        for (GuiWidget* widget : mChildren)
            processed = widget->updateMouseMoved(offsetPosition, processed) || processed;
        processed = onHover(position, processed) || processed;
    }
    return processed;
}

bool GuiScrollArea::updateMouseButtonPressed(sf::Vector2f position, bool processed)
{
    if (mVisible)
    {
        sf::Vector2f offsetPosition = sf::Vector2f(position.x, position.y + mOffset);
        if (!mFocus)
            offsetPosition = sf::Vector2f(-1.0f, 1.0f);
        for (GuiWidget* widget : mChildren)
            processed = widget->updateMouseButtonPressed(offsetPosition, processed) || processed;
        processed = onPress(position, processed) || processed;
    }
    return processed;
}

bool GuiScrollArea::updateMouseButtonReleased(sf::Vector2f position, bool processed)
{
    if (mVisible)
    {
        sf::Vector2f offsetPosition = sf::Vector2f(position.x, position.y + mOffset);
        if (!mFocus)
            offsetPosition = sf::Vector2f(-1.0f, 1.0f);
        for (GuiWidget* widget : mChildren)
            processed = widget->updateMouseButtonReleased(offsetPosition, processed) || processed;
        processed = onRelease(position, processed) || processed;
    }
    return processed;
}

void GuiScrollArea::onContentWidthChanged(float contentWidth)
{
    mContentSize.x = contentWidth;
    mInsideSize = sf::Vector2f(std::min<int>(mContentSize.x, mMaxVisibleSize.x), std::min<int>(mContentSize.y, mMaxVisibleSize.y));
    mOutsideSize = mInsideSize;
}

void GuiScrollArea::onContentHeightChanged(float contentHeight)
{
    mContentSize.y = contentHeight;
    mInsideSize = sf::Vector2f(std::min<int>(mContentSize.x, mMaxVisibleSize.x), std::min<int>(mContentSize.y, mMaxVisibleSize.y));
    mOutsideSize = mInsideSize;
    mScrollbarVisible = mContentSize.y > mInsideSize.y;
    if (mScrollbarVisible)
        mOutsideSize.x += SCROLLBAR_OFFSET + 0.5f * mScrollButton.getSize().x;
}

bool GuiScrollArea::onPress(sf::Vector2f position, bool processed)
{
    if (!processed && !mScrolling && mScrollButton.getGlobalBounds().contains(position))
    {
        mScrolling = true;
        mAnchor = position.y;
        return true;
    }
    return false;
}

bool GuiScrollArea::onRelease(sf::Vector2f /*position*/, bool /*processed*/)
{
    mScrolling = false;
    return false;
}

bool GuiScrollArea::onHover(sf::Vector2f position, bool /*processed*/)
{
    if (mScrolling)
    {
        float deltaRatio = (position.y - mAnchor) / (mInsideSize.y - mScrollButton.getSize().y);
        float ratio = clamp(getOffsetRatio() + deltaRatio, 0.0f, 1.0f);
        setOffsetRatio(ratio);
        mAnchor = position.y;
        return true;
    }
    return mFocus;
}

bool GuiScrollArea::onMouseWheelScroll(float delta, bool /*processed*/)
{
    if (mFocus && mScrollbarVisible)
    {
        mOffset = clamp(mOffset - delta * OFFSET_BY_TICK, 0.0f, mContentSize.y - mMaxVisibleSize.y);
        updateView();
        updateScrollbar();
        return true;
    }
    return false;
}

void GuiScrollArea::applyDesign()
{
    GuiWidget::applyDesign();
    mSprite.setPosition(mInsidePosition);
    mSprite.setTexture(mRenderTexture.getTexture());
    mSprite.setTextureRect(sf::IntRect(0, 0, mInsideSize.x, mInsideSize.y));
    mOffset = clamp(mOffset, 0.0f, mContentSize.y - mMaxVisibleSize.y);
    mScrollButton.setSize(sf::Vector2f(8.0f, 32.0f));
    updateView();
    updateScrollbar();
}

void GuiScrollArea::applyStyle()
{
    GuiWidget::applyStyle();
    if (mStyle)
    {
        sf::Color scrollbarColor = mStyle->getFirstChildByName("scrollbar").getAttributes().get<sf::Color>("color", sf::Color());
        mLine[0].color = scrollbarColor;
        mLine[1].color = scrollbarColor;
        mScrollButton.setFillColor(mStyle->getFirstChildByName("scrollbutton").getAttributes().get<sf::Color>("color", sf::Color()));
        mScrollButton.setOutlineThickness(mStyle->getFirstChildByName("scrollbutton").getAttributes().get<float>("borderSize", 0.0f));
        mScrollButton.setOutlineColor(mStyle->getFirstChildByName("scrollbutton").getAttributes().get<sf::Color>("borderColor", sf::Color()));
    }
}

void GuiScrollArea::updateView()
{
    mRenderTexture.setView(sf::View(sf::FloatRect(int(mInsidePosition.x), int(mOffset + mInsidePosition.y),
        mMaxVisibleSize.x, mMaxVisibleSize.y))); // Use integral origin to prevent blurry texts
}

void GuiScrollArea::updateScrollbar()
{
    mLine[0].position = mInsidePosition + sf::Vector2f(mInsideSize.x + SCROLLBAR_OFFSET, 0.0f);
    mLine[1].position = mInsidePosition + sf::Vector2f(mInsideSize.x + SCROLLBAR_OFFSET, mInsideSize.y);
    float scrollButtonOffsetX = mInsideSize.x + SCROLLBAR_OFFSET - 0.5f * mScrollButton.getSize().x;
    float scrollbuttonOffsetY = getOffsetRatio() * (mInsideSize.y - mScrollButton.getSize().y);
    mScrollButton.setPosition(mInsidePosition + sf::Vector2f(scrollButtonOffsetX, scrollbuttonOffsetY));
}

float GuiScrollArea::getOffsetRatio() const
{
    if (mScrollbarVisible)
        return mOffset / (mContentSize.y - mMaxVisibleSize.y);
    return 0.0f;
}

void GuiScrollArea::setOffsetRatio(float ratio)
{
    mOffset = ratio * (mContentSize.y - mMaxVisibleSize.y);
    updateView();
    updateScrollbar();
}
