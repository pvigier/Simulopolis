#include "ImmigrantsWindow.h"
#include "resource/StylesheetManager.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiButton.h"
#include "gui/GuiTable.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "city/Person.h"
#include "city/Market.h"
#include "util/format.h"

ImmigrantsWindow::ImmigrantsWindow(Id listenerId, StylesheetManager* stylesheetManager,
    std::vector<Person*> immigrants, int year, Market<Lease>* market) :
    GuiWindow("Immigrants", stylesheetManager->getStylesheet("window")), mListenerId(listenerId),
    mStylesheetManager(stylesheetManager), mImmigrants(std::move(immigrants)), mYear(year), mMarket(market),
    mTable(nullptr), mText(nullptr)
{

}

ImmigrantsWindow::~ImmigrantsWindow()
{
    //dtor
}

void ImmigrantsWindow::setUp()
{
    // Create table
    std::vector<std::string> names{"Name", "Age", "Money", "Visa"};
    mTable = mGui->createWithDefaultName<GuiTable>(names, mStylesheetManager->getStylesheet("table"));

    // Text
    mText = mGui->createWithDefaultName<GuiText>("", 12, mStylesheetManager->getStylesheet("darkText"));
    onNewMonth();

    // Window
    add(mTable);
    add(mText);
    setPosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    // Add rows
    for (Person* immigrant : mImmigrants)
        addImmigrant(immigrant, true);

    subscribe(mListenerId);
}

void ImmigrantsWindow::addImmigrant(Person* person, bool alreadyAdded)
{
    if (!alreadyAdded)
        mImmigrants.push_back(person);

    std::string fullName = person->getFullName();

    // Visa
    GuiWidget* visaButtons = mGui->createWithDefaultName<GuiWidget>();
    visaButtons->setLayout(std::make_unique<GuiHBoxLayout>(2.0f));
    GuiButton* acceptedButton = mGui->create<GuiButton>("Accepted" + std::to_string(person->getId()) + "|" + visaButtons->getName(), mStylesheetManager->getStylesheet("button"));
    acceptedButton->setLayout(std::make_unique<GuiHBoxLayout>(0.0f, GuiLayout::Margins{2.0f, 2.0f, 2.0f, 2.0f}));
    acceptedButton->add(mGui->createWithDefaultName<GuiText>("Yes", 12, mStylesheetManager->getStylesheet("darkText")));
    acceptedButton->subscribe(mListenerId);
    GuiButton* rejectedButton = mGui->create<GuiButton>("Rejected" + std::to_string(person->getId()) + "|" + visaButtons->getName(), mStylesheetManager->getStylesheet("button"));
    rejectedButton->add(mGui->createWithDefaultName<GuiText>("No", 12, mStylesheetManager->getStylesheet("darkText")));
    rejectedButton->setLayout(std::make_unique<GuiHBoxLayout>(0.0f, GuiLayout::Margins{2.0f, 2.0f, 2.0f, 2.0f}));
    rejectedButton->subscribe(mListenerId);
    visaButtons->add(acceptedButton);
    visaButtons->add(rejectedButton);

    // Add row
    mTable->addRow({
        mGui->createWithDefaultName<GuiText>(fullName, 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(format("%d", person->getAge(mYear)), 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>(format("%.2f", person->getMoney()), 12, mStylesheetManager->getStylesheet("darkText")),
        visaButtons
    });
}

void ImmigrantsWindow::removeImmigrant(Person* person)
{
    std::size_t i = std::find(mImmigrants.begin(), mImmigrants.end(), person) - mImmigrants.begin();
    mTable->removeRow(i);
    mImmigrants.erase(mImmigrants.begin() + i);
}

void ImmigrantsWindow::onNewMonth()
{
    mText->setString(format("Housing available: %d", mMarket->getItems().size()));
}

void ImmigrantsWindow::onNewYear()
{
    ++mYear;
    for (std::size_t i = 0; i < mImmigrants.size(); ++i)
        static_cast<GuiText*>(mTable->getCellContent(i, 1))->setString(format("%d", mImmigrants[i]->getAge(mYear)));
}
