#include "GuiScrollArea.h"
#include "util/common.h"

GuiScrollArea::GuiScrollArea(sf::Vector2i maxVisibleSize) :
    GuiWidget(), mFocus(false), mScrollbarVisible(false), mOffset(0.0f), mMaxVisibleSize(maxVisibleSize),
    mScrolling(false), mAnchor(0.0f)
{
    mRenderTexture.create(mMaxVisibleSize.x, mMaxVisibleSize.y);
    mSprite.setTexture(mRenderTexture.getTexture());
    mScrollButton.setSize(sf::Vector2f(8.0f, 32.0f));
}

GuiScrollArea::GuiScrollArea(const PropertyList& properties) : GuiWidget(properties)
{

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
        for (GuiWidget* widget : mChildren)
            processed = widget->updateMouseMoved(position, processed) || processed;
        processed = onHover(position, processed) || processed;
    }
    return processed;
}

bool GuiScrollArea::updateMouseButtonPressed(sf::Vector2f position, bool processed)
{
    if (mVisible)
    {
        for (GuiWidget* widget : mChildren)
            processed = widget->updateMouseButtonPressed(position, processed) || processed;
        processed = onPress(position, processed) || processed;
    }
    return processed;
}

bool GuiScrollArea::updateMouseButtonReleased(sf::Vector2f position, bool processed)
{
    if (mVisible)
    {
        for (GuiWidget* widget : mChildren)
            processed = widget->updateMouseButtonReleased(position, processed) || processed;
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

void GuiScrollArea::onPositionChanged()
{
    GuiWidget::onPositionChanged();
    // Content
    updateView();
    mSprite.setPosition(mPosition);
    // Scroll bar
    updateScrollbar();
}

void GuiScrollArea::onContentSizeChanged(sf::Vector2f contentSize)
{
    mContentSize = contentSize;
    mSize = sf::Vector2f(std::min<int>(mContentSize.x, mMaxVisibleSize.x), std::min<int>(mContentSize.y, mMaxVisibleSize.y));
    mSprite.setTextureRect(sf::IntRect(0, 0, mSize.x, mSize.y));
    mScrollbarVisible = contentSize.y > mSize.y;
    updateView();
    updateScrollbar();
    GuiWidget::onContentSizeChanged(mSize);
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
        float deltaRatio = (position.y - mAnchor) / (mSize.y - mScrollButton.getSize().y);
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

void GuiScrollArea::updateView()
{
    mRenderTexture.setView(sf::View(sf::FloatRect(mPosition.x, mOffset + mPosition.y, mMaxVisibleSize.x, mMaxVisibleSize.y)));
}

void GuiScrollArea::updateScrollbar()
{
    mLine[0].position = mPosition + sf::Vector2f(mSize.x + SCROLLBAR_OFFSET, 0.0f);
    mLine[1].position = mPosition + sf::Vector2f(mSize.x + SCROLLBAR_OFFSET, mSize.y);
    float scrollButtonOffsetX = mSize.x + SCROLLBAR_OFFSET - 0.5f * mScrollButton.getSize().x;
    float scrollbuttonOffsetY = getOffsetRatio() * (mSize.y - mScrollButton.getSize().y);
    mScrollButton.setPosition(mPosition + sf::Vector2f(scrollButtonOffsetX, scrollbuttonOffsetY));
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
