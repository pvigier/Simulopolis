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

CitizensWindow::CitizensWindow(StylesheetManager* stylesheetManager) :
    GuiWindow("Citizens", stylesheetManager->getStylesheet("window")),
    mStylesheetManager(stylesheetManager), mTable(nullptr)
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
}

void CitizensWindow::addCitizen(Person* person, int year)
{
    std::string fullName = person->getFullName();

    // Add row
    mTable->addRow({
        mGui->createWithDefaultName<GuiText>(fullName, 12, mStylesheetManager->getStylesheet("button")),
        mGui->createWithDefaultName<GuiText>(std::to_string(person->getAge(year)), 12, mStylesheetManager->getStylesheet("button")),
    });
}

void CitizensWindow::removeCitizen(const std::string& fullName)
{

}

