#include "render/RenderEngine.h"

#include <iostream>

RenderEngine::RenderEngine() : mWindow(sf::VideoMode(800, 600), "City Builder",
    sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings(0, 0, 4))
{
    mWindow.setFramerateLimit(60);
    mRenderTexture.create(800, 600);
    mSprite.setTexture(mRenderTexture.getTexture());
}

RenderEngine::~RenderEngine()
{

}

sf::RenderWindow& RenderEngine::getWindow()
{
    return mWindow;
}

bool RenderEngine::isWindowOpen() const
{
    return mWindow.isOpen();
}

void RenderEngine::closeWindow()
{
    mWindow.close();
}

sf::Vector2i RenderEngine::getViewportSize() const
{
    return sf::Vector2i(mRenderTexture.getSize());
}

sf::Vector2i RenderEngine::getViewportOffset() const
{
    return mViewportOffset;
}

const sf::View& RenderEngine::getView() const
{
    return mRenderTexture.getView();
}

void RenderEngine::setView(const sf::View& view)
{
    mRenderTexture.setView(view);
}

sf::Vector2f RenderEngine::mapPixelToCoords(const sf::Vector2i &point, const sf::View &view) const
{
    return mRenderTexture.mapPixelToCoords(point, view);
}

void RenderEngine::clear()
{
    mWindow.clear();
    mRenderTexture.clear();
    mViewportOffset = sf::Vector2i(mWindow.getSize()) / 2 - sf::Vector2i(mRenderTexture.getSize()) / 2;
    mSprite.setPosition(sf::Vector2f(mViewportOffset));
}

void RenderEngine::draw(const sf::Drawable& drawable)
{
    mRenderTexture.draw(drawable);
}

void RenderEngine::display()
{
    mRenderTexture.display();
    mWindow.draw(mSprite);
    mWindow.display();
}
