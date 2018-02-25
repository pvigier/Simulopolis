#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "util/NonCopyable.h"
#include "util/NonMovable.h"
#include "message/Mailbox.h"

class MessageBus;
class InputEngine;
class GuiWidget;

using GuiWidgetPtr = std::shared_ptr<GuiWidget>;

class Gui : public NonCopyable, public NonMovable, public sf::Transformable, public sf::Drawable
{
public:
    Gui(sf::Vector2f windowSize);
    ~Gui();

    static void setMessageBus(MessageBus* messageBus);
    static void setInputEngine(InputEngine* inputEngine);

    // Draw the menu.
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    template<typename T, typename... Args>
    std::shared_ptr<T> create(const std::string& name, Args&&... args)
    {
        std::shared_ptr<T> widget(new T(args...));
        mWidgets[name] = widget;
        return widget;
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> createRoot(const std::string& name, Args&&... args)
    {
        std::shared_ptr<T> widget(new T(args...));
        mWidgets[name] = widget;
        mRootWidgets.push_back(widget);
        return widget;
    }

    GuiWidgetPtr get(const std::string& name);
    const GuiWidgetPtr get(const std::string& name) const;

    template<typename T>
    std::shared_ptr<T> get(const std::string& name)
    {
        return std::static_pointer_cast<T>(mWidgets[name]);
    }

    template <typename T>
    std::shared_ptr<T> get(const std::string& name) const
    {
        return std::static_pointer_cast<T>(mWidgets.at(name));
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
    std::unordered_map<std::string, GuiWidgetPtr> mWidgets;
    std::vector<GuiWidgetPtr> mRootWidgets;
};
