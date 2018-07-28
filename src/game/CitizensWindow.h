#pragma once

#include "gui/GuiWindow.h"

class StylesheetManager;
class GuiTable;
class Person;

class CitizensWindow : public GuiWindow
{
public:
    CitizensWindow(StylesheetManager* stylesheetManager,
        const std::vector<std::unique_ptr<Person>>& citizens, int year);
    ~CitizensWindow();

    virtual void setUp() override;

    void addCitizen(Person* person);
    void removeCitizen(const std::string& fullName);

private:
    StylesheetManager* mStylesheetManager;
    const std::vector<std::unique_ptr<Person>>& mCitizens;
    int mYear;
    GuiTable* mTable;
};
