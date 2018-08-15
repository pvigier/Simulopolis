#include "gui/GuiInput.h"

GuiInput::GuiInput(unsigned int characterSize, const XmlDocument* style) :
    GuiText("", characterSize, style), mCursorShape(sf::Vector2f(1.0f, characterSize*5/4)),
    mElapsedTime(0.0f), mRegex(".*")
{
    setCursor(0);
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
    setCursor(mCursor);
}

void GuiInput::setRegex(const std::string& s)
{
    mRegex.assign(s);
}

void GuiInput::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    mElapsedTime = (mElapsedTime + mClock.restart().asMilliseconds()) % 2000;
    GuiText::render(target, states);
    if (mElapsedTime < 1000)
        target.draw(mCursorShape);
}

bool GuiInput::onKey(sf::Keyboard::Key key, bool processed)
{
    if (key == sf::Keyboard::Left && mCursor > 0)
    {
        setCursor(mCursor - 1);
        return true;
    }
    else if (key == sf::Keyboard::Right && mCursor < mText.getString().getSize())
    {
        setCursor(mCursor + 1);
        return true;
    }
    return false;
}

bool GuiInput::onText(sf::Uint32 unicode, bool processed)
{
    sf::String s = mText.getString();
    if (unicode == 8)
    {
        if (s.getSize() > 0 && mCursor > 0)
        {
            s.erase(mCursor - 1);
            if (updateText(s))
                setCursor(mCursor - 1);
        }
    }
    else if (unicode >= 32)
    {
        s.insert(mCursor, unicode);
        if (updateText(s))
            setCursor(mCursor + 1);
    }
    return true;
}

void GuiInput::setCursor(std::size_t cursor)
{
    mCursor = cursor;
    // Move cursor
    if (mCursor >= mText.getString().getSize())
        mCursorShape.setPosition(sf::Vector2f(mText.getGlobalBounds().left + mText.getGlobalBounds().width, mText.getPosition().y));
    else
        mCursorShape.setPosition(mText.findCharacterPos(mCursor));
    // Reset clock
    mElapsedTime = 0;
    mClock.restart();
}

bool GuiInput::updateText(const sf::String& text)
{
    if (std::regex_match(text.toAnsiString(), mRegex))
    {
        GuiText::setText(text);
        return true;
    }
    return false;
}
