#include "gui/GuiText.h"
#include "resource/PropertyList.h"

GuiText::GuiText(const std::string& text, unsigned int characterSize, const GuiStyle& style) :
    mText(text, *style.font, characterSize)
{
    mText.setFillColor(style.textColor);
    computeSize();
}

GuiText::GuiText(const PropertyList& properties) : GuiWidget(properties)
{
    const GuiStyle& style = properties.get<const GuiStyle&>("style");
    mText.setString(properties.get<std::string>("text", ""));
    mText.setFont(*style.font);
    mText.setCharacterSize(properties.get<unsigned int>("characterSize", 0));
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
    computeSize();
    setDirty();
}

void GuiText::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mText);
}

void GuiText::computeSize()
{
    mSize = sf::Vector2f(mText.getGlobalBounds().width, mText.getGlobalBounds().height);
}
