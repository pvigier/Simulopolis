#pragma once

#include "gui/GuiWindow.h"

class StylesheetManager;
class GuiTable;
class Person;

class ImmigrantsWindow : public GuiWindow
{
public:
    ImmigrantsWindow(StylesheetManager* stylesheetManager,
        const std::vector<std::unique_ptr<Person>>& immigrants, int year);
    ~ImmigrantsWindow();

    virtual void setUp() override;

    void addImmigrant(Person* person);
    void removeImmigrant(const std::string& fullName);

private:
    StylesheetManager* mStylesheetManager;
    const std::vector<std::unique_ptr<Person>>& mImmigrants;
    int mYear;
    GuiTable* mTable;
};
