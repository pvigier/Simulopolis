#include "CitizensWindow.h"
#include "resource/StylesheetManager.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiButton.h"
#include "gui/GuiTable.h"
#include "gui/GuiScrollArea.h"
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

    // Scroll area
    GuiScrollArea* scrollArea = mGui->createWithDefaultName<GuiScrollArea>(sf::Vector2i(400, 200));
    scrollArea->add(mTable);
    scrollArea->setLayout(std::make_unique<GuiVBoxLayout>());

    // Window
    add(scrollArea);
    setOutsidePosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    // Add rows
    for (Person* citizen : mCitizens)
        addCitizen(citizen, true);

    subscribe(mListenerId);
}

void CitizensWindow::addCitizen(Person* person, bool alreadyAdded)
{
    if (!alreadyAdded)
        mCitizens.push_back(person);

    std::string fullName = person->getFullName();

    // Person button
    GuiWidget* personButton = mGui->create<GuiButton>("openPersonWindow" + std::to_string(person->getId()) + "|" + mTable->getName(), mStylesheetManager->getStylesheet("button"));
    personButton->setLayout(std::make_unique<GuiHBoxLayout>(0.0f, GuiLayout::Margins{2.0f, 2.0f, 2.0f, 2.0f}));
    personButton->add(mGui->createWithDefaultName<GuiText>(fullName, 12, mStylesheetManager->getStylesheet("darkText")));
    personButton->subscribe(mListenerId);

    // Add row
    mTable->addRow({
        personButton,
        mGui->createWithDefaultName<GuiText>("", 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>("", 12, mStylesheetManager->getStylesheet("darkText")),
    });
}

void CitizensWindow::removeCitizen(Person* person)
{
    std::size_t i = std::find(mCitizens.begin(), mCitizens.end(), person) - mCitizens.begin();
    mTable->removeRow(i);
    mCitizens.erase(mCitizens.begin() + i);
}

void CitizensWindow::update()
{
    for (std::size_t i = 0; i < mCitizens.size(); ++i)
    {
        static_cast<GuiText*>(mTable->getCellContent(i, 1))->setString(format("%d", mCitizens[i]->getAge(mYear)));
        static_cast<GuiText*>(mTable->getCellContent(i, 2))->setString(mCitizens[i]->getWorkStatus());
    }
}

void CitizensWindow::onNewYear()
{
    ++mYear;
}
