#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "util/NonCopyable.h"
#include "util/NonMovable.h"
#include "message/Mailbox.h"

class GuiWidget;
class MessageBus;
class InputEngine;

class Gui : public NonCopyable, public NonMovable, public sf::Transformable, public sf::Drawable
{
public:
    Gui();
    ~Gui();

    static void setMessageBus(MessageBus* messageBus);
    static void setInputEngine(InputEngine* inputEngine);

    // Draw the menu.
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void add(const std::string& name, std::unique_ptr<GuiWidget> widget);
    void addRoot(const std::string& name, std::unique_ptr<GuiWidget> widget);

    template<typename T, typename... Args>
    T* create(const std::string& name, Args&&... args)
    {
        add(name, std::make_unique<T>(args...));
        return get<T>(name);
    }

    template<typename T, typename... Args>
    T* createRoot(const std::string& name, Args&&... args)
    {
        addRoot(name, std::make_unique<T>(args...));
        return get<T>(name);
    }

    GuiWidget* get(const std::string& name);
    const GuiWidget* get(const std::string& name) const;

    template<typename T>
    T* get(const std::string& name)
    {
        return static_cast<T*>(mWidgets[name].get());
    }

    template <typename T>
    T* get(const std::string& name) const
    {
        return static_cast<T*>(mWidgets.at(name).get());
    }

    void remove(const std::string& name);
    void remove(GuiWidget* widget);

    void update();
    void handleMessages();

    void setWindowSize(sf::Vector2f windowSize);
    void setVisible(bool visible);
    bool isVisible() const;

private:
    static MessageBus* sMessageBus;
    static InputEngine* sInputEngine;
    Mailbox mMailbox;
    sf::View mView;
    bool mVisible;
    std::unordered_map<std::string, std::unique_ptr<GuiWidget>> mWidgets;
    std::vector<GuiWidget*> mRootWidgets;
};
