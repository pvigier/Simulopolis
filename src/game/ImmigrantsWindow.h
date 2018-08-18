#pragma once

#include "gui/GuiWindow.h"

class StylesheetManager;
class GuiTable;
class GuiText;
class Person;
class Lease;
template<typename T> class Market;

class ImmigrantsWindow : public GuiWindow
{
public:
    ImmigrantsWindow(Id listenerId, StylesheetManager* stylesheetManager,
        std::vector<Person*> immigrants, int year, const Market<Lease>* market);
    ~ImmigrantsWindow();

    virtual void setUp() override;

    void addImmigrant(Person* person, bool alreadyAdded = false);
    void removeImmigrant(Person* person);

    void onNewMonth();
    void onNewYear();

private:
    Id mListenerId;
    StylesheetManager* mStylesheetManager;
    std::vector<Person*> mImmigrants;
    int mYear;
    const Market<Lease>* mMarket;
    GuiTable* mTable;
    GuiText* mText;
};
