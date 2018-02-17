#pragma once

#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "util/NonCopyable.h"
#include "util/NonMovable.h"
#include "message/Mailbox.h"

class MessageBus;
class InputEngine;
class GuiWidget;

class Gui : public NonCopyable, public NonMovable, public sf::Transformable, public sf::Drawable
{
public:
    Gui(sf::Vector2f windowSize);
    ~Gui();

    static void setMessageBus(MessageBus* messageBus);
    static void setInputEngine(InputEngine* inputEngine);

    // Draw the menu.
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void add(const std::string& name, GuiWidget* widget);
    void addRoot(const std::string& name, GuiWidget* widget);
    GuiWidget* get(const std::string& name);
    const GuiWidget* get(const std::string& name) const;

    template<typename T>
    T* get(const std::string& name)
    {
            return static_cast<T*>(mWidgets[name]);
    }

    template <typename T>
    const T* get(const std::string& name) const
    {
        return static_cast<T*>(mWidgets.at(name));
    }

    void update();

    void setVisible(bool visible);
    bool isVisible() const;

private:
    static MessageBus* sMessageBus;
    static InputEngine* sInputEngine;
    Mailbox mMailbox;
    sf::View mView;
    bool mVisible;
    std::unordered_map<std::string, GuiWidget*> mWidgets;
    std::vector<GuiWidget*> mRootWidgets;
};
