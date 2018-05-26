#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "message/Subject.h"

class GuiWidget;
class GuiLayout;
class PropertyList;

class GuiWidget : public sf::Drawable, public Subject
{
public:
    GuiWidget();
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
    virtual void setSize(sf::Vector2f size);
    virtual sf::FloatRect getRect() const;
    bool isVisible() const;
    void setVisible(bool visible);
    bool isDirty() const;

    // Events
    bool updateMouseMoved(sf::Vector2f position, bool processed);
    bool updateMouseButtonPressed(sf::Vector2f position, bool processed);
    bool updateMouseButtonReleased(sf::Vector2f position, bool processed);

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

    void setDirty();

    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const;

    // Events
    virtual bool onHover(sf::Vector2f position, bool processed);
    virtual bool onPress(sf::Vector2f position, bool processed);
    virtual bool onRelease(sf::Vector2f position, bool processed);

private:
    bool mDirty;
};
