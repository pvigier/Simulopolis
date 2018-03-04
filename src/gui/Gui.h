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
        mWidgets[name] = std::unique_ptr<T>(new T(args...));
        return mWidgets[name].get();
    }

    template<typename T, typename... Args>
    T* createRoot(const std::string& name, Args&&... args)
    {
        mWidgets[name] = std::unique_ptr<T>(new T(args...));
        mRootWidgets.push_back(mWidgets[name].get());
        return mWidgets[name].get();
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

    void update();

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
