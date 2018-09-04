#pragma once

#include "message/Mailbox.h"
#include "gui/GuiWindow.h"

class StylesheetManager;
class GuiTable;
class City;
class Person;

class CitizensWindow : public GuiWindow
{
public:
    CitizensWindow(Id listenerId, MessageBus* messageBus, StylesheetManager* stylesheetManager, City& city);
    ~CitizensWindow();

    virtual void setUp() override;

    void update();

private:
    Id mListenerId;
    MessageBus* mMessageBus;
    StylesheetManager* mStylesheetManager;
    Mailbox mMailbox;
    City& mCity;
    std::vector<Person*> mCitizens;
    GuiTable* mTable;

    void addCitizen(Person* person);
    void removeCitizen(Person* person);
};
