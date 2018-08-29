#include "gui/Gui.h"
#include "message/MessageBus.h"
#include "input/InputEngine.h"
#include "gui/GuiWidget.h"
#include "gui/GuiWindow.h"
#include "gui/GuiEvent.h"

MessageBus* Gui::sMessageBus = nullptr;
InputEngine* Gui::sInputEngine = nullptr;

Gui::Gui() : mVisible(true), mCounter(0)
{
    // Register the mailbox
    sMessageBus->addMailbox(mMailbox);
    sInputEngine->subscribe(mMailbox.getId());
}

Gui::~Gui()
{
    // Remove widgets
    for (int i = mRootWidgets.size() - 1; i >= 0; --i)
        remove(mRootWidgets[i]);

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
        widget->render(target, states, sf::FloatRect(sf::Vector2f(), mView.getSize()));
}

void Gui::add(const std::string& name, std::unique_ptr<GuiWidget> widget)
{
    if (widget->hasGuiEvents())
        widget->subscribe(mMailbox.getId());
    widget->setGui(this);
    widget->setName(name);
    widget->setRoot(false);
    mWidgets[name] = std::move(widget);
    mWidgets[name]->setUp();
}

void Gui::addRoot(const std::string& name, std::unique_ptr<GuiWidget> widget)
{
    if (widget->hasGuiEvents())
        widget->subscribe(mMailbox.getId());
    widget->setGui(this);
    widget->setName(name);
    widget->setRoot(true);
    mRootWidgets.push_back(widget.get());
    mWidgets[name] = std::move(widget);
    mWidgets[name]->setUp();
}

void Gui::addWithDefaultName(std::unique_ptr<GuiWidget> widget)
{
    add(generateName(), std::move(widget));
}

void Gui::addRootWithDefaultName(std::unique_ptr<GuiWidget> widget)
{
    addRoot(generateName(), std::move(widget));
}

GuiWidget* Gui::get(const std::string& name)
{
    return mWidgets[name].get();
}

const GuiWidget* Gui::get(const std::string& name) const
{
    return mWidgets.at(name).get();
}

void Gui::remove(const std::string& name)
{
    remove(mWidgets[name].get());
}

void Gui::remove(GuiWidget* widget)
{
    widget->tearDown();
    if (widget->isRoot())
        mRootWidgets.erase(std::find(mRootWidgets.begin(), mRootWidgets.end(), widget));
    for (GuiWidget* child : widget->getChildren())
        remove(child);
    mWidgets.erase(widget->getName());
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
        if (message.type == MessageType::INPUT)
        {
            sf::Event event = message.getInfo<sf::Event>();
            bool processed = false;
            switch (event.type)
            {
                case sf::Event::Resized:
                    mView.setSize(event.size.width, event.size.height);
                    break;
                case sf::Event::MouseMoved:
                    for (int i = mRootWidgets.size() - 1; i >= 0; --i)
                        processed = mRootWidgets[i]->updateMouseMoved(mousePosition, processed) || processed;
                    break;
                case sf::Event::MouseButtonPressed:
                    for (int i = mRootWidgets.size() - 1; i >= 0; --i)
                        processed = mRootWidgets[i]->updateMouseButtonPressed(mousePosition, processed) || processed;
                    break;
                case sf::Event::MouseButtonReleased:
                    for (int i = mRootWidgets.size() - 1; i >= 0; --i)
                        processed = mRootWidgets[i]->updateMouseButtonReleased(mousePosition, processed) || processed;
                    break;
                case sf::Event::MouseWheelMoved:
                    for (int i = mRootWidgets.size() - 1; i >= 0; --i)
                        processed = mRootWidgets[i]->updateMouseWheelScrolled(event.mouseWheel.delta, processed) || processed;
                case sf::Event::TextEntered:
                    for (int i = mRootWidgets.size() - 1; i >= 0; --i)
                        processed = mRootWidgets[i]->updateTextEntered(event.text.unicode, processed) || processed;
                    break;
                case sf::Event::KeyPressed:
                    for (int i = mRootWidgets.size() - 1; i >= 0; --i)
                        processed = mRootWidgets[i]->updateKeyPressed(event.key.code, processed) || processed;
                    break;
                default:
                    break;
            }
            if (!processed)
                notify(message);
        }
        else if (message.type == MessageType::GUI)
        {
            GuiEvent event = message.getInfo<GuiEvent>();
            switch (event.type)
            {
                case GuiEvent::Type::WINDOW_CLOSED:
                    remove(event.widget);
                    break;
                default:
                    break;
            }
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

std::string Gui::generateName()
{
    std::string name = std::to_string(mCounter);
    while (mWidgets.find(name) != mWidgets.end())
        name = std::to_string(++mCounter);
    return name;
}
