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

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual void setUp();
    void update();
    virtual void tearDown();

    void add(GuiWidget* widget);
    void insert(std::size_t i, GuiWidget* widget);
    GuiWidget* remove(std::size_t i);
    std::vector<GuiWidget*>& getChildren();
    const std::vector<GuiWidget*>& getChildren() const;

    virtual void updateSize();
    void fitSizeToContent();
    sf::Vector2f getContentSize() const;

    // Parameters
    void setGui(Gui* gui);
    const std::string& getName() const;
    void setName(const std::string& name);
    bool isRoot() const;
    void setRoot(bool root);
    const GuiWidget* getParent() const;
    void setParent(GuiWidget* parent);
    void setLayout(std::unique_ptr<GuiLayout> layout);
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f position);
    sf::Vector2f getSize() const;
    void setFixedSize(sf::Vector2f size);
    sf::FloatRect getRect() const;
    void setBackgroundColor(const sf::Color& color);
    void setBorderSize(int borderSize);
    bool isVisible() const;
    void setVisible(bool visible);
    bool isDirty() const;

    // Input
    virtual bool updateMouseMoved(sf::Vector2f position, bool processed);
    virtual bool updateMouseButtonPressed(sf::Vector2f position, bool processed);
    virtual bool updateMouseButtonReleased(sf::Vector2f position, bool processed);
    bool updateMouseWheelScrolled(float delta, bool processed);
    bool updateKeyPressed(sf::Keyboard::Key key, bool processed);
    bool updateTextEntered(sf::Uint32 unicode, bool processed);

    virtual bool hasGuiEvents() const;

protected:
    Gui* mGui;
    std::string mName;
    bool mRoot;
    GuiWidget* mParent;
    std::vector<GuiWidget*> mChildren;
    std::unique_ptr<GuiLayout> mLayout;
    sf::Vector2f mPosition;
    sf::Vector2f mSize;
    bool mVisible;
    bool mFixedSize;
    const XmlDocument* mStyle;
    sf::RectangleShape mBackground;

    void setDirty();
    void updateAlignment();
    void resetDirty();

    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const;

    // Events
    virtual void onPositionChanged();
    virtual void onContentSizeChanged(sf::Vector2f contentSize);
    virtual void onSizeFixed();

    // Input
    virtual bool onHover(sf::Vector2f position, bool processed);
    virtual bool onPress(sf::Vector2f position, bool processed);
    virtual bool onRelease(sf::Vector2f position, bool processed);
    virtual bool onMouseWheelScroll(float delta, bool processed);
    virtual bool onKey(sf::Keyboard::Key key, bool processed);
    virtual bool onText(sf::Uint32 unicode, bool processed);

    virtual void applyStyle();

private:
    bool mDirty;
};
