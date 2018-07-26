#pragma once

#include <SFML/Graphics/RenderTexture.hpp>

class StylesheetManager;
class Gui;
class GuiWidget;
class GuiWindow;
class GuiTable;
class Person;

class ImmigrantsWindow
{
public:
    ImmigrantsWindow(Gui* gui, StylesheetManager* stylesheetManager);
    ~ImmigrantsWindow();

    void addImmigrant(Person* person, int year);
    void removeImmigrant(const std::string& fullName);

private:
    enum Column : int {NAME, AGE};

    Gui* mGui;
    StylesheetManager* mStylesheetManager;
    GuiWindow* mWindow;
    GuiTable* mTable;
};
