#pragma once

#include "gui/GuiWindow.h"

class StylesheetManager;
class GuiTable;
class Person;

class CitizensWindow : public GuiWindow
{
public:
    CitizensWindow(Id listenerId, StylesheetManager* stylesheetManager,
        const std::vector<Person*>& citizens, int year);
    ~CitizensWindow();

    virtual void setUp() override;

    void addCitizen(const Person* person);
    void removeCitizen(const std::string& fullName);

private:
    Id mListenerId;
    StylesheetManager* mStylesheetManager;
    const std::vector<Person*>& mCitizens;
    int mYear;
    GuiTable* mTable;
};
