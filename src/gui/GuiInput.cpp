#include "gui/GuiInput.h"
#include "resource/XmlDocument.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"

GuiInput::GuiInput(unsigned int characterSize, const XmlDocument* style) :
    GuiWidget(style), mFocus(false), mCharacterSize(characterSize), mText(nullptr),
    mCursorShape(sf::Vector2f(1.0f, mCharacterSize * 5 / 4)), mElapsedTime(0.0f), mRegex(".*")
{

}

GuiInput::GuiInput(const PropertyList& properties) : GuiWidget(properties)
{

}

GuiInput::~GuiInput()
{
    //dtor
}

void GuiInput::setUp()
{
    mText = mGui->createWithDefaultName<GuiText>("", mCharacterSize, mStyle->getFirstChildByName("text").getAttributes().get<const XmlDocument*>("style"));
    add(mText);
    setCursor(0);
}

const sf::String& GuiInput::getString() const
{
    return mText->getString();
}

bool GuiInput::setString(const sf::String& text)
{
    if (std::regex_match(text.toAnsiString(), mRegex))
    {
        mText->setString(text);
        return true;
    }
    return false;
}

void GuiInput::setRegex(const std::string& s)
{
    mRegex.assign(s);
}

void GuiInput::render(sf::RenderTarget& target, sf::RenderStates states) const
{
    GuiWidget::render(target, states);
    mElapsedTime = (mElapsedTime + mClock.restart().asMilliseconds()) % 2000;
    if (mFocus && mElapsedTime < 1000)
        target.draw(mCursorShape, states);
}

void GuiInput::onOutsidePositionChanged()
{
    GuiWidget::onOutsidePositionChanged();
    setCursor(mCursor);
}

bool GuiInput::onPress(sf::Vector2f position, bool processed)
{
    if (processed || !mBackground.getGlobalBounds().contains(position))
    {
        mFocus = false;
        return false;
    }
    else
    {
        mFocus = true;
        resetClock();
        setCursor(mousePositionToCursor(position));
        return true;
    }
}

bool GuiInput::onKey(sf::Keyboard::Key key, bool processed)
{
    if (mFocus && !processed)
    {
        if (key == sf::Keyboard::Left && mCursor > 0)
            setCursor(mCursor - 1);
        else if (key == sf::Keyboard::Right && mCursor < mText->getText().getString().getSize())
            setCursor(mCursor + 1);
        return true;
    }
    return false;
}

bool GuiInput::onText(sf::Uint32 unicode, bool processed)
{
    if (mFocus && !processed)
    {
        sf::String s = mText->getString();
        if (unicode == 8)
        {
            if (s.getSize() > 0 && mCursor > 0)
            {
                s.erase(mCursor - 1);
                if (setString(s))
                    setCursor(mCursor - 1);
            }
        }
        else if (unicode >= 32)
        {
            s.insert(mCursor, unicode);
            if (setString(s))
                setCursor(mCursor + 1);
        }
        return true;
    }
    return false;
}

void GuiInput::resetClock() const
{
    mElapsedTime = 0;
    mClock.restart();
}

void GuiInput::setCursor(std::size_t cursor)
{
    mCursor = cursor;
    // Move cursor
    const sf::Text& text = mText->getText();
    if (mCursor >= mText->getString().getSize())
        mCursorShape.setPosition(sf::Vector2f(text.getGlobalBounds().left + text.getGlobalBounds().width, text.getPosition().y));
    else
        mCursorShape.setPosition(text.findCharacterPos(mCursor));
    // Reset clock
    resetClock();
}

std::size_t GuiInput::mousePositionToCursor(sf::Vector2f position)
{
    std::size_t size = mText->getString().getSize();
    float prevX = 0.0f;
    for (std::size_t i = 0; i < size; ++i)
    {
        float x = mText->getText().findCharacterPos(i).x;
        if (position.x < x)
        {
            if (i == 0)
                return 0;
            else if (position.x - prevX < x - position.x)
                return i - 1;
            else
                return i;
        }
        prevX = x;
    }
    return size;
}
