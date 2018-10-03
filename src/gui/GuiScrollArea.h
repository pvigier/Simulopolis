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

    virtual void render(sf::RenderTarget& target, sf::RenderStates states, const sf::FloatRect& viewport) const override;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual void setViewportSize(sf::Vector2u viewportSize) override;

    virtual bool updateMouseMoved(sf::Vector2f position, bool processed) override;
    virtual bool updateMouseButtonPressed(sf::Vector2f position, bool processed) override;
    virtual bool updateMouseButtonReleased(sf::Vector2f position, bool processed) override;

protected:
    virtual void onContentWidthChanged(float contentWidth) override;
    virtual void onContentHeightChanged(float contentHeight) override;

    virtual bool onPress(sf::Vector2f position, bool processed) override;
    virtual bool onRelease(sf::Vector2f position, bool processed) override;
    virtual bool onHover(sf::Vector2f position, bool processed) override;
    virtual bool onMouseWheelScroll(float delta, bool processed) override;

    virtual void applyDesign() override;
    virtual void applyStyle() override;

private:
    bool mFocus;
    bool mScrollbarVisible;
    float mOffset;
    std::array<SizePolicy, 2> mMaxVisibleSizePolicies;
    sf::Vector2i mMaxVisibleSize;
    sf::Vector2f mMaxVisibleSizeRatio;
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
