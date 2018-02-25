#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "message/Subject.h"

class GuiWidget;
class GuiLayout;

using GuiWidgetPtr = std::shared_ptr<GuiWidget>;

class GuiWidget : public sf::Drawable, public Subject
{
public:
    GuiWidget();
    virtual ~GuiWidget();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final override;

    virtual void update();

    virtual void add(GuiWidgetPtr widget);
    std::vector<GuiWidgetPtr>& getChildren();
    const std::vector<GuiWidgetPtr>& getChildren() const;

    void fitSizeToContent();

    // Parameters
    void setLayout(std::unique_ptr<GuiLayout> layout);
    virtual sf::Vector2f getPosition() const;
    virtual void setPosition(sf::Vector2f position);
    virtual sf::Vector2f getSize() const;
    virtual void setSize(sf::Vector2f size);
    virtual sf::FloatRect getRect() const;
    bool isVisible() const;
    void setVisible(bool visible);

    // Events
    void updateMouseMoved(sf::Vector2f position);
    void updateMouseButtonPressed(sf::Vector2f position);
    void updateMouseButtonReleased(sf::Vector2f position);

protected:
    std::vector<GuiWidgetPtr> mChildren;
    std::unique_ptr<GuiLayout> mLayout;
    sf::Vector2f mPosition;
    sf::Vector2f mSize;
    bool mVisible;

    virtual void render(sf::RenderTarget& target, sf::RenderStates states) const;

    // Events
    virtual void onHover(sf::Vector2f position);
    virtual void onPress(sf::Vector2f position);
    virtual void onRelease(sf::Vector2f position);
};
