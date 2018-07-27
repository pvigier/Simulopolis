#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include "gui/GuiWindow.h"

class StylesheetManager;
class Gui;
class Person;
class GuiImage;

class PersonWindow : public GuiWindow
{
public:
    PersonWindow(StylesheetManager* stylesheetManager, const Person& person, int year);
    ~PersonWindow();

    virtual void setUp() override;

    sf::RenderTexture& getRenderTexture();
    sf::View getView();

private:
    StylesheetManager* mStylesheetManager;
    const Person& mPerson;
    int mYear;
    GuiImage* mImage;
    sf::RenderTexture mRenderTexture;
};
