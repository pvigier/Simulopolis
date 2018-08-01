#include "CitizensWindow.h"
#include "resource/StylesheetManager.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiButton.h"
#include "gui/GuiTable.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "city/Person.h"
#include "util/format.h"

CitizensWindow::CitizensWindow(Id listenerId, StylesheetManager* stylesheetManager,
    std::vector<Person*> citizens, int year) :
    GuiWindow("Citizens", stylesheetManager->getStylesheet("window")), mListenerId(listenerId),
    mStylesheetManager(stylesheetManager), mCitizens(std::move(citizens)), mYear(year), mTable(nullptr)
{

}

CitizensWindow::~CitizensWindow()
{
    //dtor
}

void CitizensWindow::setUp()
{
    // Create table
    std::vector<std::string> names{"Name", "Age", "Work"};
    mTable = mGui->createWithDefaultName<GuiTable>(names, mStylesheetManager->getStylesheet("table"));

    // Window
    add(mTable);
    setPosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    // Add rows
    for (Person* citizen : mCitizens)
        addCitizen(citizen, true);
}

void CitizensWindow::addCitizen(Person* person, bool alreadyAdded)
{
    if (!alreadyAdded)
        mCitizens.push_back(person);

    std::string fullName = person->getFullName();

    // Person button
    GuiWidget* personButton = mGui->create<GuiButton>("openPersonWindow" + std::to_string(person->getId()) + "|" + mTable->getName(), mStylesheetManager->getStylesheet("button"));
    personButton->setLayout(std::make_unique<GuiHBoxLayout>(0.0f, GuiLayout::Margins{2.0f, 2.0f, 2.0f, 2.0f}));
    personButton->add(mGui->createWithDefaultName<GuiText>(fullName, 12, mStylesheetManager->getStylesheet("button")));
    personButton->subscribe(mListenerId);

    // Add row
    mTable->addRow({
        personButton,
        mGui->createWithDefaultName<GuiText>(format("%d", person->getAge(mYear)), 12, mStylesheetManager->getStylesheet("button")),
        mGui->createWithDefaultName<GuiText>(person->getWorkStatus(), 12, mStylesheetManager->getStylesheet("button")),
    });

    subscribe(mListenerId);
}

void CitizensWindow::removeCitizen(Person* person)
{
    std::size_t i = std::find(mCitizens.begin(), mCitizens.end(), person) - mCitizens.begin();
    mTable->removeRow(i);
    mCitizens.erase(mCitizens.begin() + i);
}

