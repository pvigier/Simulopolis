#pragma once

#include "gui/GuiWindow.h"

class StylesheetManager;
class Gui;
class GuiWidget;
class GuiTable;
class Person;

class ImmigrantsWindow : public GuiWindow
{
public:
    ImmigrantsWindow(StylesheetManager* stylesheetManager);
    ~ImmigrantsWindow();

    virtual void setUp() override;

    void addImmigrant(Person* person, int year);
    void removeImmigrant(const std::string& fullName);

private:
    enum Column : int {NAME, AGE};

    StylesheetManager* mStylesheetManager;
    GuiTable* mTable;
};
