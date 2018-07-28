#include "ImmigrantsWindow.h"
#include "resource/StylesheetManager.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiButton.h"
#include "gui/GuiWindow.h"
#include "gui/GuiTable.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "city/Person.h"

ImmigrantsWindow::ImmigrantsWindow(StylesheetManager* stylesheetManager,
    const std::vector<std::unique_ptr<Person>>& immigrants, int year) :
    GuiWindow("Immigrants", stylesheetManager->getStylesheet("window")),
    mStylesheetManager(stylesheetManager), mImmigrants(immigrants), mYear(year), mTable(nullptr)
{

}

ImmigrantsWindow::~ImmigrantsWindow()
{
    //dtor
}

void ImmigrantsWindow::setUp()
{
    // Create table
    std::vector<std::string> names{"Name", "Age", "Visa"};
    mTable = mGui->createWithDefaultName<GuiTable>(names, mStylesheetManager->getStylesheet("table"));

    // Window
    add(mTable);
    setPosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    // Add rows
    for (const std::unique_ptr<Person>& immigrant : mImmigrants)
        addImmigrant(immigrant.get());
}

void ImmigrantsWindow::addImmigrant(Person* person)
{
    std::string fullName = person->getFullName();

    // Visa
    GuiWidget* visaButtons = mGui->createWithDefaultName<GuiWidget>();
    visaButtons->setLayout(std::make_unique<GuiHBoxLayout>(2.0f));
    GuiButton* acceptedButton = mGui->create<GuiButton>(fullName + "Accepted" + "Button", mStylesheetManager->getStylesheet("button"));
    acceptedButton->setLayout(std::make_unique<GuiHBoxLayout>(0.0f, GuiLayout::Margins{2.0f, 2.0f, 2.0f, 2.0f}));
    acceptedButton->add(mGui->createWithDefaultName<GuiText>("Yes", 12, mStylesheetManager->getStylesheet("button")));
    GuiButton* rejectedButton = mGui->create<GuiButton>(fullName + "Rejected" + "Button", mStylesheetManager->getStylesheet("button"));
    rejectedButton->add(mGui->createWithDefaultName<GuiText>("No", 12, mStylesheetManager->getStylesheet("button")));
    rejectedButton->setLayout(std::make_unique<GuiHBoxLayout>(0.0f, GuiLayout::Margins{2.0f, 2.0f, 2.0f, 2.0f}));
    visaButtons->add(acceptedButton);
    visaButtons->add(rejectedButton);

    // Add row
    mTable->addRow({
        mGui->createWithDefaultName<GuiText>(fullName, 12, mStylesheetManager->getStylesheet("button")),
        mGui->createWithDefaultName<GuiText>(std::to_string(person->getAge(mYear)), 12, mStylesheetManager->getStylesheet("button")),
        visaButtons
    });
}

void ImmigrantsWindow::removeImmigrant(const std::string& fullName)
{

}

