#include "GuiEntry.h"

GuiEntry::GuiEntry()
{

}

GuiEntry::GuiEntry(sf::RectangleShape shape, sf::Text text, const std::string& message) :
    mShape(shape), mText(text), mMessage(message)
{

}

sf::RectangleShape& GuiEntry::getShape()
{
    return mShape;
}

const sf::RectangleShape& GuiEntry::getShape() const
{
    return mShape;
}

sf::Text& GuiEntry::getText()
{
    return mText;
}

const sf::Text& GuiEntry::getText() const
{
    return mText;
}

std::string GuiEntry::getMessage() const
{
    return mMessage;
}
