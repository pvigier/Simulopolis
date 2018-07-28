#include "CitizensWindow.h"
#include "resource/StylesheetManager.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiButton.h"
#include "gui/GuiWindow.h"
#include "gui/GuiTable.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "city/Person.h"
#include "util/format.h"

CitizensWindow::CitizensWindow(StylesheetManager* stylesheetManager,
    const std::vector<std::unique_ptr<Person>>& citizens, int year) :
    GuiWindow("Citizens", stylesheetManager->getStylesheet("window")),
    mStylesheetManager(stylesheetManager), mCitizens(citizens), mYear(year), mTable(nullptr)
{

}

CitizensWindow::~CitizensWindow()
{
    //dtor
}

void CitizensWindow::setUp()
{
    // Create table
    std::vector<std::string> names{"Name", "Age"};
    mTable = mGui->createWithDefaultName<GuiTable>(names, mStylesheetManager->getStylesheet("table"));

    // Window
    add(mTable);
    setPosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    // Add rows
    for (const std::unique_ptr<Person>& citizen : mCitizens)
        addCitizen(citizen.get());
}

void CitizensWindow::addCitizen(Person* person)
{
    std::string fullName = person->getFullName();

    // Person button
    GuiWidget* personButton = mGui->create<GuiButton>("openWindow" + fullName + mName, mStylesheetManager->getStylesheet("button"));
    personButton->setLayout(std::make_unique<GuiHBoxLayout>(0.0f, GuiLayout::Margins{2.0f, 2.0f, 2.0f, 2.0f}));
    personButton->add(mGui->createWithDefaultName<GuiText>(fullName, 12, mStylesheetManager->getStylesheet("button")));

    // Add row
    mTable->addRow({
        personButton,
        mGui->createWithDefaultName<GuiText>(format("%d", person->getAge(mYear)), 12, mStylesheetManager->getStylesheet("button")),
    });
}

void CitizensWindow::removeCitizen(const std::string& fullName)
{

}

