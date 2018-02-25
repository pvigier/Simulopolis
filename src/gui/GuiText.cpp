#include "GuiText.h"

GuiText::GuiText(const std::string& text, unsigned int characterSize, const GuiStyle& style) :
    mText(text, *style.font, characterSize)
{
    mText.setFillColor(style.textColor);
    computeSize();
}

GuiText::~GuiText()
{
    //dtor
}

void GuiText::setPosition(sf::Vector2f position)
{
    sf::Vector2f offset(sf::Vector2f(mText.getGlobalBounds().left, mText.getGlobalBounds().top) - mText.getPosition());
    position = sf::Vector2f(sf::Vector2i(position));
    GuiWidget::setPosition(position);
    mText.setPosition(position - offset);
}

void GuiText::setCharacterSize(unsigned int characterSize)
{
    mText.setCharacterSize(characterSize);
    computeSize();
}

void GuiText::setText(const std::string& text)
{
    mText.setString(text);
}

void GuiText::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mText);
}

void GuiText::computeSize()
{
    mSize = sf::Vector2f(mText.getGlobalBounds().width, mText.getGlobalBounds().height);
}
