#pragma once

#include "gui/GuiWindow.h"

class StylesheetManager;
class GuiTable;
class Person;

class CitizensWindow : public GuiWindow
{
public:
    CitizensWindow(Id listenerId, StylesheetManager* stylesheetManager,
        std::vector<Person*> citizens, int year);
    ~CitizensWindow();

    virtual void setUp() override;

    void addCitizen(Person* person, bool alreadyAdded = false);
    void removeCitizen(Person* person);

    void update();
    void onNewYear();

private:
    Id mListenerId;
    StylesheetManager* mStylesheetManager;
    std::vector<Person*> mCitizens;
    int mYear;
    GuiTable* mTable;
};
