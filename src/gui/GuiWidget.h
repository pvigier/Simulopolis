#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "message/Subject.h"

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

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final override;

    void update();

    void add(GuiWidget* widget);
    void insert(std::size_t i, GuiWidget* widget);
    std::vector<GuiWidget*>& getChildren();
    const std::vector<GuiWidget*>& getChildren() const;

    void fitSizeToContent();

    // Parameters
    const std::string& getName() const;
    void setName(const std::string& name);
    bool isRoot() const;
    void setRoot(bool root);
    void setParent(GuiWidget* parent);
    void setLayout(std::unique_ptr<GuiLayout> layout);
    virtual sf::Vector2f getPosition() const;
    virtual void setPosition(sf::Vector2f position);
    virtual sf::Vector2f getSize() const;
    void setFixedSize(sf::Vector2f size);
    virtual sf::FloatRect getRect() const;
    void setBackgroundColor(const sf::Color& color);
    void setBorderSize(int borderSize);
    bool isVisible() const;
    void setVisible(bool visible);
    bool isDirty() const;

    // Events
    bool updateMouseMoved(sf::Vector2f position, bool processed);
    bool updateMouseButtonPressed(sf::Vector2f position, bool processed);
    bool updateMouseButtonReleased(sf::Vector2f position, bool processed);
    bool updateTextEntered(sf::Uint32 unicode, bool processed);

    virtual bool hasGuiEvents() const;

protected:
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
    virtual void setSize(sf::Vector2f size);
    public: void updateSize();
    protected: void updateAlignment();

    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const;

    // Events
    virtual bool onHover(sf::Vector2f position, bool processed);
    virtual bool onPress(sf::Vector2f position, bool processed);
    virtual bool onRelease(sf::Vector2f position, bool processed);
    virtual bool onText(sf::Uint32 unicode, bool processed);

    virtual void applyStyle();

private:
    bool mDirty;
};
