#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "message/Subject.h"

class GuiWidget;
class GuiLayout;

class GuiWidget : public sf::Drawable, public Subject
{
public:
    GuiWidget();
    virtual ~GuiWidget();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final override;

    void update();

    void add(GuiWidget* widget);
    std::vector<GuiWidget*>& getChildren();
    const std::vector<GuiWidget*>& getChildren() const;

    void fitSizeToContent();

    // Parameters
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
    void updateMouseMoved(sf::Vector2f position);
    void updateMouseButtonPressed(sf::Vector2f position);
    void updateMouseButtonReleased(sf::Vector2f position);

protected:
    GuiWidget* mParent;
    std::vector<GuiWidget*> mChildren;
    std::unique_ptr<GuiLayout> mLayout;
    sf::Vector2f mPosition;
    sf::Vector2f mSize;
    bool mVisible;

    void setDirty();

    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const;

    // Events
    virtual void onHover(sf::Vector2f position);
    virtual void onPress(sf::Vector2f position);
    virtual void onRelease(sf::Vector2f position);

private:
    bool mDirty;
};
