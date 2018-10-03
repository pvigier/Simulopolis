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

#include <vector>
#include <SFML/Graphics.hpp>
#include "message/Subject.h"

class Gui;
class GuiWidget;
class GuiLayout;
class XmlDocument;
class PropertyList;

class GuiWidget : public sf::Drawable, public Subject
{
public:
    GuiWidget(const XmlDocument* style = nullptr);
    GuiWidget(const PropertyList& properties);
    virtual ~GuiWidget();

    virtual void render(sf::RenderTarget& target, sf::RenderStates states, const sf::FloatRect& viewport) const;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual void setUp();
    void update();
    virtual void tearDown();

    // Hierarchy
    void add(GuiWidget* widget);
    void insert(std::size_t i, GuiWidget* widget);
    GuiWidget* remove(std::size_t i);
    void remove(GuiWidget* widget);
    std::vector<GuiWidget*>& getChildren();
    const std::vector<GuiWidget*>& getChildren() const;
    bool isRoot() const;
    void setRoot(bool root);
    GuiWidget* getParent();
    const GuiWidget* getParent() const;
    void setParent(GuiWidget* parent);

    // Parameters
    void setGui(Gui* gui);
    const std::string& getName() const;
    void setName(const std::string& name);
    void setLayout(std::unique_ptr<GuiLayout> layout);
    bool isDirty() const;

    // Positions and sizes
    sf::Vector2f getOutsidePosition() const;
    void setOutsidePosition(sf::Vector2f position);
    sf::Vector2f getInsidePosition() const;
    sf::Vector2f getOutsideSize() const;
    sf::Vector2f getInsideSize() const;
    void setFixedInsideWidth(float width);
    void setFixedInsideHeight(float height);
    void setFixedInsideSize(sf::Vector2f size);
    void fitInsideWidthToContent();
    void fitInsideHeightToContent();
    void fitInsideSizeToContent();
    sf::Vector2f getContentSize() const;
    sf::FloatRect getOutsideRect() const;
    virtual void updateSize(); // Public only for GuiTable

    // Style
    void setBackgroundColor(const sf::Color& color);
    void setBorderSize(int borderSize);
    bool isVisible() const;
    void setVisible(bool visible);

    // Input
    virtual bool updateMouseMoved(sf::Vector2f position, bool processed);
    virtual bool updateMouseButtonPressed(sf::Vector2f position, bool processed);
    virtual bool updateMouseButtonReleased(sf::Vector2f position, bool processed);
    bool updateMouseWheelScrolled(float delta, bool processed);
    bool updateKeyPressed(sf::Keyboard::Key key, bool processed);
    bool updateTextEntered(sf::Uint32 unicode, bool processed);

    virtual bool hasGuiEvents() const;

protected:
    enum SizePolicy{FIXED, FIT_TO_CONTENT, RATIO};

    // Hierarchy
    bool mRoot;
    GuiWidget* mParent;
    std::vector<GuiWidget*> mChildren;
    std::unique_ptr<GuiLayout> mLayout;

    // Parameters
    Gui* mGui;
    std::string mName;

    // Position and size
    sf::Vector2f mOutsidePosition;
    sf::Vector2f mInsidePosition;
    std::array<SizePolicy, 2> mSizePolicies;
    sf::Vector2f mOutsideSize;
    sf::Vector2f mInsideSize;
    sf::Vector2f mInsideSizeRatio;

    // Style
    bool mVisible;
    const XmlDocument* mStyle;
    sf::RectangleShape mBackground;

    // Dirtiness
    void setDirty();
    void resetDirty();

    // Events
    virtual void onOutsidePositionChanged();
    virtual void onContentWidthChanged(float contentWidth);
    virtual void onContentHeightChanged(float contentHeight);
    virtual void onInsideWidthFixed();
    virtual void onInsideHeightFixed();
    virtual void onViewportSizeChanged(sf::Vector2u viewportSize);

    // Input
    virtual bool onHover(sf::Vector2f position, bool processed);
    virtual bool onPress(sf::Vector2f position, bool processed);
    virtual bool onRelease(sf::Vector2f position, bool processed);
    virtual bool onMouseWheelScroll(float delta, bool processed);
    virtual bool onKey(sf::Keyboard::Key key, bool processed);
    virtual bool onText(sf::Uint32 unicode, bool processed);

    // Updates
    void updateAlignment();
    void updateDesign();

    // Design and style
    virtual void applyDesign();
    virtual void applyStyle();

private:
    // Flag
    bool mDirty;
};
