#include "GuiScrollArea.h"
#include "resource/XmlDocument.h"
#include "util/common.h"

GuiScrollArea::GuiScrollArea(sf::Vector2i maxVisibleSize, const XmlDocument* style) :
    GuiWidget(style), mFocus(false), mScrollbarVisible(false), mOffset(0.0f), mMaxVisibleSize(maxVisibleSize),
    mScrolling(false), mAnchor(0.0f)
{
    mRenderTexture.create(mMaxVisibleSize.x, mMaxVisibleSize.y);
    mSprite.setTexture(mRenderTexture.getTexture());
    mScrollButton.setSize(sf::Vector2f(8.0f, 32.0f));
    applyStyle();
}

GuiScrollArea::GuiScrollArea(const PropertyList& properties) :
    GuiWidget(properties), mFocus(false), mScrollbarVisible(false), mOffset(0.0f), mScrolling(false), mAnchor(0.0f)
{
    mMaxVisibleSize = properties.get<sf::Vector2i>("maxVisibleSize", sf::Vector2i());
    mRenderTexture.create(mMaxVisibleSize.x, mMaxVisibleSize.y);
    mSprite.setTexture(mRenderTexture.getTexture());
    mScrollButton.setSize(sf::Vector2f(8.0f, 32.0f));
    applyStyle();
}

void GuiScrollArea::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mVisible)
    {
        mRenderTexture.clear();
        for (const GuiWidget* widget : mChildren)
            mRenderTexture.draw(*widget);
        mRenderTexture.display();
        render(target, states);
    }
}

bool GuiScrollArea::updateMouseMoved(sf::Vector2f position, bool processed)
{
    if (mVisible)
    {
        sf::Vector2f offsetPosition = sf::Vector2f(position.x, position.y + mOffset);
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
        for (GuiWidget* widget : mChildren)
            processed = widget->updateMouseButtonReleased(offsetPosition, processed) || processed;
        processed = onRelease(position, processed) || processed;
    }
    return processed;
}

void GuiScrollArea::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
    // Scrollbar
    if (mScrollbarVisible)
    {
        target.draw(mLine.data(), 2, sf::Lines);
        target.draw(mScrollButton);
    }
}

void GuiScrollArea::onOutsidePositionChanged()
{
    GuiWidget::onOutsidePositionChanged();
    // Content
    updateView();
    mSprite.setPosition(mInsidePosition);
    // Scroll bar
    updateScrollbar();
}

void GuiScrollArea::onContentSizeChanged(sf::Vector2f contentSize)
{
    mContentSize = contentSize;
    mInsideSize = sf::Vector2f(std::min<int>(mContentSize.x, mMaxVisibleSize.x), std::min<int>(mContentSize.y, mMaxVisibleSize.y));
    mOutsideSize = mInsideSize;
    mSprite.setTextureRect(sf::IntRect(0, 0, mInsideSize.x, mInsideSize.y));
    mScrollbarVisible = contentSize.y > mInsideSize.y;
    if (mScrollbarVisible)
        mOutsideSize.x += SCROLLBAR_OFFSET + 0.5f * mScrollButton.getSize().x;
    mOffset = clamp(mOffset, 0.0f, mContentSize.y - mMaxVisibleSize.y);
    updateView();
    updateScrollbar();
    mBackground.setSize(mOutsideSize);
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

bool GuiScrollArea::onRelease(sf::Vector2f position, bool processed)
{
    mScrolling = false;
    return false;
}

bool GuiScrollArea::onHover(sf::Vector2f position, bool processed)
{
    mFocus = mBackground.getGlobalBounds().contains(position);
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

bool GuiScrollArea::onMouseWheelScroll(float delta, bool processed)
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

void GuiScrollArea::applyStyle()
{
    sf::Color scrollbarColor = mStyle->getFirstChildByName("scrollbar").getAttributes().get<sf::Color>("color", sf::Color());
    mLine[0].color = scrollbarColor;
    mLine[1].color = scrollbarColor;
    mScrollButton.setFillColor(mStyle->getFirstChildByName("scrollbutton").getAttributes().get<sf::Color>("color", sf::Color()));
    mScrollButton.setOutlineThickness(mStyle->getFirstChildByName("scrollbutton").getAttributes().get<float>("borderSize", 0.0f));
    mScrollButton.setOutlineColor(mStyle->getFirstChildByName("scrollbutton").getAttributes().get<sf::Color>("borderColor", sf::Color()));
}

void GuiScrollArea::updateView()
{
    mRenderTexture.setView(sf::View(sf::FloatRect(mInsidePosition.x, mOffset + mInsidePosition.y, mMaxVisibleSize.x, mMaxVisibleSize.y)));
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
