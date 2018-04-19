#include "gui/GuiWindow.h"
#include "resource/ResourceManager.h"

GuiWindow::GuiWindow(sf::Vector2f size, const std::string& title, const XmlDocument* style) :
    mStyle(style), mOnMove(false)
{
    setSize(size);
    setTitle(title);
    applyStyle();
}

GuiWindow::GuiWindow(const PropertyList& properties) :
    GuiWidget(properties), mStyle(properties.get<const XmlDocument*>("style"))
{
    setSize(mSize);
    setPosition(mPosition);
    setTitle(properties.get("title"));
    applyStyle();
}

void GuiWindow::setPosition(sf::Vector2f position)
{
    GuiWidget::setPosition(position);
    mBody.setPosition(position);
    mBar.setPosition(position);
    // Close button
    sf::Vector2f closeButtonSize = sf::Vector2f(mCloseButton.getGlobalBounds().width, mCloseButton.getGlobalBounds().height);
    mCloseButton.setPosition(position + sf::Vector2f(mSize.x - closeButtonSize.x * 1.5f, -mBar.getSize().y * 0.5f - closeButtonSize.y * 0.5f));
    // Title
    sf::Vector2f offset(sf::Vector2f(mTitle.getGlobalBounds().left, mTitle.getGlobalBounds().top) - mTitle.getPosition());
    sf::Vector2f titleSize = sf::Vector2f(mTitle.getGlobalBounds().width, mTitle.getGlobalBounds().height);
    sf::Vector2i titlePosition(position + sf::Vector2f(mSize.x * 0.5f, -mBar.getSize().y * 0.5f) - offset - titleSize * 0.5f);
    mTitle.setPosition(sf::Vector2f(titlePosition));
}

void GuiWindow::setSize(sf::Vector2f size)
{
    GuiWidget::setSize(size);
    mBody.setSize(size);
    mBar.setSize(sf::Vector2f(size.x, mStyle->getFirstChildByName("bar").getAttributes().get<float>("height")));
    mCloseButton.setRadius(mStyle->getFirstChildByName("bar").getAttributes().get<float>("height") * 0.25f);
    // Update position
    setPosition(mPosition);
}

void GuiWindow::setTitle(const std::string& title)
{
    mTitle.setString(title);
}

void GuiWindow::onHover(sf::Vector2f position)
{
    if (mOnMove)
    {
        setPosition(mPosition + position - mAnchor);
        mAnchor = position;
    }
    if (mCloseButton.getGlobalBounds().contains(position))
        mCloseButton.setFillColor(mStyle->getFirstChildByName("close").getAttributes().get<sf::Color>("highlightColor"));
    else
        mCloseButton.setFillColor(mStyle->getFirstChildByName("close").getAttributes().get<sf::Color>("color"));
}

void GuiWindow::onPress(sf::Vector2f position)
{
    if (mBar.getGlobalBounds().contains(position))
    {
        mOnMove = true;
        mAnchor = position;
    }
}

void GuiWindow::onRelease(sf::Vector2f position)
{
    mOnMove = false;
}

void GuiWindow::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mBody);
    target.draw(mBar);
    target.draw(mCloseButton);
    target.draw(mTitle);
}

void GuiWindow::applyStyle()
{
    // Body
    mBody.setOutlineThickness(-mStyle->getFirstChildByName("border").getAttributes().get<int>("size"));
    mBody.setFillColor(mStyle->getFirstChildByName("body").getAttributes().get<sf::Color>("color"));
    mBody.setOutlineColor(mStyle->getFirstChildByName("border").getAttributes().get<sf::Color>("color"));
    // Bar
    mBar.setOrigin(0.0f, mStyle->getFirstChildByName("bar").getAttributes().get<float>("height") -
        mStyle->getFirstChildByName("border").getAttributes().get<int>("size"));
    mBar.setOutlineThickness(-mStyle->getFirstChildByName("border").getAttributes().get<int>("size"));
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
