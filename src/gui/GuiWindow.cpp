#include "gui/GuiWindow.h"
#include "resource/ResourceManager.h"
#include "gui/GuiEvent.h"

GuiWindow::GuiWindow(const std::string& title, const XmlDocument* style) :
    GuiWidget(style), mOnMove(false)
{
    applyStyle();
    setTitle(title);
}

GuiWindow::GuiWindow(const PropertyList& properties) :
    GuiWidget(properties)
{
    applyStyle();
    setSize(mSize);
    setPosition(mPosition);
    setTitle(properties.get("title"));
}

void GuiWindow::setPosition(sf::Vector2f position)
{
    GuiWidget::setPosition(position);
    mBar.setPosition(position);
    sf::Vector2f barSize = sf::Vector2f(mBar.getGlobalBounds().width, mBar.getGlobalBounds().height);
    // Close button
    sf::Vector2f closeButtonSize = sf::Vector2f(mCloseButton.getGlobalBounds().width, mCloseButton.getGlobalBounds().height);
    mCloseButton.setPosition(position + sf::Vector2f(mSize.x - closeButtonSize.x * 1.5f, -barSize.y * 0.5f - closeButtonSize.y * 0.5f));
    // Title
    sf::Vector2f offset(sf::Vector2f(mTitle.getGlobalBounds().left, mTitle.getGlobalBounds().top) - mTitle.getPosition());
    sf::Vector2f titleSize = sf::Vector2f(mTitle.getGlobalBounds().width, mTitle.getGlobalBounds().height);
    sf::Vector2i titlePosition(position + sf::Vector2f(mSize.x * 0.5f, -barSize.y * 0.5f) - offset - titleSize * 0.5f);
    mTitle.setPosition(sf::Vector2f(titlePosition));
}

void GuiWindow::setTitle(const std::string& title)
{
    mTitle.setString(title);
}

bool GuiWindow::hasGuiEvents() const
{
    return true;
}

bool GuiWindow::onHover(sf::Vector2f position, bool processed)
{
    if (mOnMove)
    {
        setPosition(mPosition + position - mAnchor);
        mAnchor = position;
    }
    return mBackground.getGlobalBounds().contains(position) || mBar.getGlobalBounds().contains(position);
}

bool GuiWindow::onPress(sf::Vector2f position, bool processed)
{
    if (!processed && mBar.getGlobalBounds().contains(position) && !mCloseButton.getGlobalBounds().contains(position))
    {
        mOnMove = true;
        mAnchor = position;
    }
    else if (!processed && mCloseButton.getGlobalBounds().contains(position))
        mCloseButton.setFillColor(mStyle->getFirstChildByName("close").getAttributes().get<sf::Color>("highlightColor"));
    return mBackground.getGlobalBounds().contains(position) || mBar.getGlobalBounds().contains(position);
}

bool GuiWindow::onRelease(sf::Vector2f position, bool processed)
{
    mOnMove = false;
    if (!processed && mCloseButton.getGlobalBounds().contains(position))
        notify(Message::create(MessageType::GUI, GuiEvent(this, GuiEvent::Type::WINDOW_CLOSED)));
    mCloseButton.setFillColor(mStyle->getFirstChildByName("close").getAttributes().get<sf::Color>("color"));
    return mBackground.getGlobalBounds().contains(position) || mBar.getGlobalBounds().contains(position);
}

void GuiWindow::setSize(sf::Vector2f size)
{
    GuiWidget::setSize(size);
    mBar.setSize(sf::Vector2f(size.x, mStyle->getFirstChildByName("bar").getAttributes().get<float>("height")));
    mCloseButton.setRadius(mStyle->getFirstChildByName("bar").getAttributes().get<float>("height") * 0.25f);
    // Update position
    setPosition(mPosition);
}

void GuiWindow::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    GuiWidget::render(target, states);
    target.draw(mBar);
    target.draw(mCloseButton);
    target.draw(mTitle);
}

void GuiWindow::applyStyle()
{
    // Bar
    mBar.setOrigin(0.0f, mStyle->getFirstChildByName("bar").getAttributes().get<float>("height") +
        mStyle->getFirstChildByName("border").getAttributes().get<int>("size"));
    mBar.setOutlineThickness(mStyle->getFirstChildByName("border").getAttributes().get<int>("size"));
    mBar.setFillColor(mStyle->getFirstChildByName("bar").getAttributes().get<sf::Color>("color"));
    mBar.setOutlineColor(mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("color"));
    // Close button
    //mCloseButton.setOutlineThickness(-1.0f);
    mCloseButton.setFillColor(mStyle->getFirstChildByName("close").getAttributes().get<sf::Color>("color"));
    //mCloseButton.setOutlineColor(mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("color"));
    // Title
    mTitle.setFont(mStyle->getFirstChildByName("title").getAttributes().get<const sf::Font&>("font"));
    mTitle.setCharacterSize(mStyle->getFirstChildByName("title").getAttributes().get<int>("characterSize"));
}
