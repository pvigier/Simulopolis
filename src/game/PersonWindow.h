#pragma once

#include <SFML/Graphics/RenderTexture.hpp>

class StylesheetManager;
class Gui;
class Person;
class GuiWindow;
class GuiImage;

class PersonWindow
{
public:
    PersonWindow(Gui* gui, StylesheetManager* stylesheetManager, const std::string& windowId,
        const Person& person, int year);
    ~PersonWindow();

    GuiWindow* getWindow();
    sf::RenderTexture& getRenderTexture();
    sf::View getView();

private:
    const Person& mPerson;
    GuiWindow* mWindow;
    GuiImage* mImage;
    sf::RenderTexture mRenderTexture;
};
