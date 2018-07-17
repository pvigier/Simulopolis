#include "gui/GuiInput.h"

GuiInput::GuiInput(unsigned int characterSize, const XmlDocument* style) :
    GuiText("", characterSize, style), mCursor(0)
{

}

GuiInput::GuiInput(const PropertyList& properties) : GuiText(properties)
{

}

GuiInput::~GuiInput()
{
    //dtor
}

void GuiInput::setPosition(sf::Vector2f position)
{
    GuiText::setPosition(position);
}


void GuiInput::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    GuiText::render(target, states);
}

bool GuiInput::onText(sf::Uint32 unicode, bool processed)
{
    sf::String s = mText.getString();
    if (unicode == 8)
    {
        if (s.getSize() > 0 && mCursor > 0)
        {
            s.erase(mCursor - 1);
            --mCursor;
        }
    }
    else
    {
        s += unicode;
        ++mCursor;
    }
    setText(s);
    return true;
}
