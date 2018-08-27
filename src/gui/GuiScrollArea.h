#pragma once

#include <SFML/Graphics.hpp>
#include "gui/GuiWidget.h"

class GuiScrollArea : public GuiWidget
{
public:
    static constexpr float OFFSET_BY_TICK = 48.0f;
    static constexpr float SCROLLBAR_OFFSET = 8.0f;

    GuiScrollArea(sf::Vector2i maxVisibleSize, const XmlDocument* style = nullptr);
    GuiScrollArea(const PropertyList& properties);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual bool updateMouseMoved(sf::Vector2f position, bool processed) override;
    virtual bool updateMouseButtonPressed(sf::Vector2f position, bool processed) override;
    virtual bool updateMouseButtonReleased(sf::Vector2f position, bool processed) override;

protected:
    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual void onOutsidePositionChanged() override;
    virtual void onContentSizeChanged(sf::Vector2f contentSize) override;
    virtual bool onPress(sf::Vector2f position, bool processed) override;
    virtual bool onRelease(sf::Vector2f position, bool processed) override;
    virtual bool onHover(sf::Vector2f position, bool processed) override;
    virtual bool onMouseWheelScroll(float delta, bool processed) override;

    virtual void applyStyle() override;

private:
    bool mFocus;
    bool mScrollbarVisible;
    float mOffset;
    sf::Vector2i mMaxVisibleSize;
    sf::Vector2f mContentSize;
    mutable sf::RenderTexture mRenderTexture;
    sf::Sprite mSprite;
    // Scrollbar
    bool mScrolling;
    float mAnchor;
    std::array<sf::Vertex, 2> mLine;
    sf::RectangleShape mScrollButton;

    void updateView();
    void updateScrollbar();
    float getOffsetRatio() const;
    void setOffsetRatio(float ratio);
};
