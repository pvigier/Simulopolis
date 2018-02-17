#include "gui/Gui.h"
#include "message/MessageBus.h"
#include "input/InputEngine.h"
#include "gui/GuiWidget.h"

MessageBus* Gui::sMessageBus = nullptr;
InputEngine* Gui::sInputEngine = nullptr;

Gui::Gui(sf::Vector2f windowSize) :
    mView(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), windowSize)), mVisible(true)
{
    // Register the mailbox
    sMessageBus->addMailbox(mMailbox);
    sInputEngine->subscribe(mMailbox.getId());
}

Gui::~Gui()
{
    // Destroy the widget
    for (auto& widget : mWidgets)
        delete widget.second;

    // Unregister the mailbox
    sInputEngine->unsubscribe(mMailbox.getId());
    sMessageBus->removeMailbox(mMailbox);
}

void Gui::setMessageBus(MessageBus* messageBus)
{
    sMessageBus = messageBus;
}

void Gui::setInputEngine(InputEngine* inputEngine)
{
    sInputEngine = inputEngine;
}


void Gui::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!mVisible)
        return;

    target.setView(mView);

    // Draw each button of the menu
    for (const GuiWidget* widget : mRootWidgets)
        target.draw(*widget);
}

void Gui::add(const std::string& name, GuiWidget* widget)
{
    mWidgets[name] = widget;
}

void Gui::addRoot(const std::string& name, GuiWidget* widget)
{
    add(name, widget);
    mRootWidgets.push_back(widget);
}

GuiWidget* Gui::get(const std::string& name)
{
    return mWidgets[name];
}

const GuiWidget* Gui::get(const std::string& name) const
{
    return mWidgets.at(name);
}

void Gui::update()
{
    sf::Vector2f mousePosition(sInputEngine->getMousePosition());
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type != MessageType::INPUT)
            continue;

        sf::Event event = message.getInfo<sf::Event>();
        switch (event.type)
        {
            case sf::Event::Resized:
                mView.setSize(event.size.width, event.size.height);
                break;
            case sf::Event::MouseMoved:
                for (GuiWidget* widget : mRootWidgets)
                    widget->hover(mousePosition);
                break;
            case sf::Event::MouseButtonPressed:
                for (GuiWidget* widget : mRootWidgets)
                    widget->click(mousePosition);
                break;
            default:
                break;
        }
    }
}

void Gui::setVisible(bool visible)
{
    mVisible = visible;
}

bool Gui::isVisible() const
{
    return mVisible;
}
