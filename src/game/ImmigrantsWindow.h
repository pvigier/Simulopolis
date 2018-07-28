#pragma once

#include "gui/GuiWindow.h"

class StylesheetManager;
class GuiTable;
class Person;

class ImmigrantsWindow : public GuiWindow
{
public:
    ImmigrantsWindow(Id listenerId, StylesheetManager* stylesheetManager,
        const std::vector<Person*>& immigrants, int year);
    ~ImmigrantsWindow();

    virtual void setUp() override;

    void addImmigrant(const Person* person);
    void removeImmigrant(const std::string& fullName);

private:
    Id mListenerId;
    StylesheetManager* mStylesheetManager;
    const std::vector<Person*>& mImmigrants;
    int mYear;
    GuiTable* mTable;
};
