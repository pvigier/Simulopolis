#pragma once

#include "gui/GuiWindow.h"

class StylesheetManager;
class GuiTable;
class Person;

class CitizensWindow : public GuiWindow
{
public:
    CitizensWindow(StylesheetManager* stylesheetManager);
    ~CitizensWindow();

    virtual void setUp() override;

    void addCitizen(Person* person, int year);
    void removeCitizen(const std::string& fullName);

private:
    StylesheetManager* mStylesheetManager;
    GuiTable* mTable;
};
