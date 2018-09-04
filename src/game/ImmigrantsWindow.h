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
    ImmigrantsWindow(Id listenerId, MessageBus* messageBus, StylesheetManager* stylesheetManager, City& city);
    ~ImmigrantsWindow();

    virtual void setUp() override;

    void update();

private:
    Id mListenerId;
    MessageBus* mMessageBus;
    StylesheetManager* mStylesheetManager;
    Mailbox mMailbox;
    City& mCity;
    std::vector<Person*> mImmigrants;
    const Market<Lease>* mRentalMarket;
    const Market<Work>* mLaborMarket;
    GuiTable* mTable;
    GuiText* mRentalMarketText;
    GuiText* mLaborMarketText;
    GuiText* mAttractivenessText;

    void addImmigrant(Person* person);
    void removeImmigrant(Person* person);
};
