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
    setTitle(properties.get("title"));
}

void GuiWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    GuiWidget::draw(target, states);
    target.draw(mBar, states);
    target.draw(mCloseButton, states);
    target.draw(mTitle, states);
}

void GuiWindow::setTitle(const std::string& title)
{
    mTitle.setString(title);
}

bool GuiWindow::hasGuiEvents() const
{
    return true;
}

void GuiWindow::onOutsidePositionChanged()
{
    GuiWidget::onOutsidePositionChanged();
    mBar.setPosition(mInsidePosition);
    sf::Vector2f barSize = sf::Vector2f(mBar.getGlobalBounds().width, mBar.getGlobalBounds().height);
    // Close button
    sf::Vector2f closeButtonSize = sf::Vector2f(mCloseButton.getGlobalBounds().width, mCloseButton.getGlobalBounds().height);
    mCloseButton.setPosition(mInsidePosition + sf::Vector2f(mInsideSize.x - closeButtonSize.x * 1.5f, -barSize.y * 0.5f - closeButtonSize.y * 0.5f));
    // Title
    sf::Vector2f titleSize = sf::Vector2f(mTitle.getGlobalBounds().width, mTitle.getCharacterSize() * 5 / 4);
    sf::Vector2i titlePosition(mInsidePosition + sf::Vector2f(mInsideSize.x * 0.5f, -barSize.y * 0.5f) - titleSize * 0.5f);
    mTitle.setPosition(sf::Vector2f(titlePosition));
}

void GuiWindow::onContentSizeChanged(sf::Vector2f contentSize)
{
    mInsideSize = contentSize;
    mBar.setSize(sf::Vector2f(mInsideSize.x, mStyle->getFirstChildByName("bar").getAttributes().get<float>("height")));
    mCloseButton.setRadius(mStyle->getFirstChildByName("bar").getAttributes().get<float>("height") * 0.25f);
    // Update position
    onOutsidePositionChanged();
    GuiWidget::onContentSizeChanged(mInsideSize); // To remove
}

bool GuiWindow::onHover(sf::Vector2f position, bool processed)
{
    if (mOnMove)
    {
        setOutsidePosition(mOutsidePosition + position - mAnchor);
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
