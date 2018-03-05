#include "gui/Gui.h"
#include "message/MessageBus.h"
#include "input/InputEngine.h"
#include "gui/GuiWidget.h"

MessageBus* Gui::sMessageBus = nullptr;
InputEngine* Gui::sInputEngine = nullptr;

Gui::Gui() : mVisible(true)
{
    // Register the mailbox
    sMessageBus->addMailbox(mMailbox);
    sInputEngine->subscribe(mMailbox.getId());
}

Gui::~Gui()
{
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
    for (GuiWidget* widget : mRootWidgets)
        target.draw(*widget);
}

void Gui::add(const std::string& name, std::unique_ptr<GuiWidget> widget)
{
    mWidgets[name] = std::move(widget);
}

void Gui::addRoot(const std::string& name, std::unique_ptr<GuiWidget> widget)
{
    mRootWidgets.push_back(widget.get());
    mWidgets[name] = std::move(widget);
}

GuiWidget* Gui::get(const std::string& name)
{
    return mWidgets[name].get();
}

const GuiWidget* Gui::get(const std::string& name) const
{
    return mWidgets.at(name).get();
}

void Gui::update()
{
    for (GuiWidget* widget : mRootWidgets)
    {
        if (widget->isDirty())
            widget->update();
    }
}

void Gui::handleMessages()
{
    // Poll events
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
                    widget->updateMouseMoved(mousePosition);
                break;
            case sf::Event::MouseButtonPressed:
                for (GuiWidget* widget : mRootWidgets)
                    widget->updateMouseButtonPressed(mousePosition);
                break;
            case sf::Event::MouseButtonReleased:
                for (GuiWidget* widget : mRootWidgets)
                    widget->updateMouseButtonReleased(mousePosition);
                break;
            default:
                break;
        }
    }
}

void Gui::setWindowSize(sf::Vector2f windowSize)
{
    mView.setCenter(windowSize * 0.5f);
    mView.setSize(windowSize);
}

void Gui::setVisible(bool visible)
{
    mVisible = visible;
}

bool Gui::isVisible() const
{
    return mVisible;
}
