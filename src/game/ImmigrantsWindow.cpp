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

#define WINDOW_ID "immigrantsWindow"
#define WIDGET_ID(suffix) WINDOW_ID suffix

ImmigrantsWindow::ImmigrantsWindow(Gui* gui, StylesheetManager* stylesheetManager) :
    mGui(gui), mStylesheetManager(stylesheetManager), mWindow(nullptr), mTable(nullptr)
{
    // Create table
    std::vector<std::string> names{"Name", "Age", "Visa"};
    mTable = mGui->create<GuiTable>(WIDGET_ID("Table"), names, mStylesheetManager->getStylesheet("table"));

    // Window
    mWindow = mGui->createRoot<GuiWindow>(WINDOW_ID, "Immigrants", mStylesheetManager->getStylesheet("window"));
    mWindow->add(mTable);
    mWindow->setPosition(sf::Vector2f(50.0f, 50.0f));
    std::unique_ptr<GuiVBoxLayout> layout(new GuiVBoxLayout(GuiLayout::HAlignment::Left, GuiLayout::VAlignment::Top, 8.0f));
    layout->setMargins(GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f});
    mWindow->setLayout(std::move(layout));
}

ImmigrantsWindow::~ImmigrantsWindow()
{
    //dtor
}

void ImmigrantsWindow::addImmigrant(Person* person, int year)
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
        mGui->create<GuiText>(WIDGET_ID("Name") + fullName, fullName, 12, mStylesheetManager->getStylesheet("button")),
        mGui->create<GuiText>(WIDGET_ID("Age") + fullName, std::to_string(person->getAge(year)), 12, mStylesheetManager->getStylesheet("button")),
        visaButtons
    });
}

void ImmigrantsWindow::removeImmigrant(const std::string& fullName)
{

}

