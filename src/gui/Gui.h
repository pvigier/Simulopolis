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
#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "message/Mailbox.h"
#include "message/Subject.h"

class GuiWidget;
class MessageBus;
class InputEngine;

class Gui : public sf::Drawable, public Subject
{
public:
    Gui();
    ~Gui();

    static void setMessageBus(MessageBus* messageBus);
    static void setInputEngine(InputEngine* inputEngine);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void add(const std::string& name, std::unique_ptr<GuiWidget> widget);
    void addRoot(const std::string& name, std::unique_ptr<GuiWidget> widget);
    void addWithDefaultName(std::unique_ptr<GuiWidget> widget);
    void addRootWithDefaultName(std::unique_ptr<GuiWidget> widget);

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

    template<typename T, typename... Args>
    T* createWithDefaultName(Args&&... args)
    {
        return create<T>(generateName(), args...);
    }

    template<typename T, typename... Args>
    T* createRootWithDefaultName(Args&&... args)
    {
        return createRoot<T>(generateName(), args...);
    }

    GuiWidget* get(const std::string& name);
    const GuiWidget* get(const std::string& name) const;

    template<typename T>
    T* get(const std::string& name)
    {
        return static_cast<T*>(mWidgets[name].get());
    }

    template<typename T>
    T* get(const std::string& name) const
    {
        return static_cast<T*>(mWidgets.at(name).get());
    }

    void remove(const std::string& name);
    void remove(GuiWidget* widget);

    void update();
    void handleMessages();

    const sf::View& getView();
    void setViewportSize(sf::Vector2i viewportSize);
    void setVisible(bool visible);
    bool isVisible() const;

    void setListen(bool listen);

private:
    static MessageBus* sMessageBus;
    static InputEngine* sInputEngine;
    Mailbox mMailbox;
    sf::View mView;
    bool mVisible;
    bool mListen;
    std::unordered_map<std::string, std::unique_ptr<GuiWidget>> mWidgets;
    std::vector<GuiWidget*> mRootWidgets;
    unsigned int mCounter; // To generate a name

    std::string generateName();
};
