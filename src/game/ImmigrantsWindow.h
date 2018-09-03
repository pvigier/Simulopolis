#pragma once

#include "message/Mailbox.h"
#include "gui/GuiWindow.h"

class StylesheetManager;
class GuiTable;
class GuiText;
class City;
class Person;
class Lease;
class Work;
template<typename T> class Market;

class ImmigrantsWindow : public GuiWindow
{
public:
    ImmigrantsWindow(Id listenerId, MessageBus* messageBus, StylesheetManager* stylesheetManager, const City& city);
    ~ImmigrantsWindow();

    virtual void setUp() override;

    void update();

    void addImmigrant(Person* person, bool alreadyAdded = false);
    void removeImmigrant(Person* person);

    void onNewYear();

private:
    Id mListenerId;
    MessageBus* mMessageBus;
    StylesheetManager* mStylesheetManager;
    Mailbox mMailbox;
    const City& mCity;
    std::vector<Person*> mImmigrants;
    int mYear;
    const Market<Lease>* mRentalMarket;
    const Market<Work>* mLaborMarket;
    GuiTable* mTable;
    GuiText* mRentalMarketText;
    GuiText* mLaborMarketText;
    GuiText* mAttractivenessText;
};
