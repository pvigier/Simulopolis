#include "render/RenderEngine.h"

RenderEngine::RenderEngine() : mWindow(sf::VideoMode(800, 600), "City Builder")
{
   mWindow.setFramerateLimit(60);
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

const sf::View& RenderEngine::getView() const
{
    return mWindow.getView();
}

void RenderEngine::setView(const sf::View& view)
{
    mWindow.setView(view);
}

void RenderEngine::clear()
{
    mWindow.clear();
}

void RenderEngine::draw(const sf::Drawable& drawable)
{
    mWindow.draw(drawable);
}

void RenderEngine::display()
{
    mWindow.display();
}
