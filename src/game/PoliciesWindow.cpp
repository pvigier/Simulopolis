#include "PoliciesWindow.h"
#include "message/MessageBus.h"
#include "resource/StylesheetManager.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiButton.h"
#include "gui/GuiTabWidget.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "gui/GuiOverlapLayout.h"
#include "gui/GuiEvent.h"
#include "util/format.h"

PoliciesWindow::PoliciesWindow(MessageBus* messageBus, StylesheetManager* stylesheetManager) :
    GuiWindow("Policies", stylesheetManager->getStylesheet("window")),
    mStylesheetManager(stylesheetManager)
{
    messageBus->addMailbox(mMailbox);
}

PoliciesWindow::~PoliciesWindow()
{
    //dtor
}

void PoliciesWindow::setUp()
{
    // Tab buttons
    std::array<std::string, 4> tabNames = {"Labor policy", "Housing policy", "Public service", "Tax policy"};
    mTabButtonsWidget = mGui->createWithDefaultName<GuiWidget>();

    for (const std::string& name : tabNames)
    {
        mTabButtons.push_back(mGui->createWithDefaultName<GuiButton>(mStylesheetManager->getStylesheet("button")));
        mTabButtons.back()->add(mGui->createWithDefaultName<GuiText>(name, 12, mStylesheetManager->getStylesheet("button")));
        mTabButtons.back()->setLayout(std::make_unique<GuiHBoxLayout>(0.0f, GuiLayout::Margins{3.0f, 3.0f, 3.0f, 3.0f}));
        mTabButtons.back()->subscribe(mMailbox.getId());
        mTabButtonsWidget->add(mTabButtons.back());
    }

    mTabButtonsWidget->setLayout(std::make_unique<GuiHBoxLayout>());

    // Labor policy
    GuiWidget* laborPolicyTab = mGui->createWithDefaultName<GuiWidget>(mStylesheetManager->getStylesheet("windowTabs"));
    laborPolicyTab->setFixedSize(sf::Vector2f(400.0f, 100.0f));

    GuiWidget* weeklyStandardWorkingHoursWidget = mGui->createWithDefaultName<GuiWidget>();
    weeklyStandardWorkingHoursWidget->add(mGui->createWithDefaultName<GuiText>("Weekly standard working hours: ", 12, mStylesheetManager->getStylesheet("button")));
    weeklyStandardWorkingHoursWidget->setLayout(std::make_unique<GuiHBoxLayout>());
    laborPolicyTab->add(weeklyStandardWorkingHoursWidget);

    GuiWidget* minimumWageWidget = mGui->createWithDefaultName<GuiWidget>();
    minimumWageWidget->add(mGui->createWithDefaultName<GuiText>("Minimum wage: ", 12, mStylesheetManager->getStylesheet("button")));
    minimumWageWidget->setLayout(std::make_unique<GuiHBoxLayout>());
    laborPolicyTab->add(minimumWageWidget);

    laborPolicyTab->setLayout(std::make_unique<GuiVBoxLayout>(4.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    // Housing policy
    GuiWidget* housingPolicyTab = mGui->createWithDefaultName<GuiWidget>(mStylesheetManager->getStylesheet("windowTabs"));
    housingPolicyTab->setFixedSize(sf::Vector2f(400.0f, 100.0f));

    GuiWidget* affordableHousingRentWidget = mGui->createWithDefaultName<GuiWidget>();
    affordableHousingRentWidget->add(mGui->createWithDefaultName<GuiText>("Rent for an affordable housing: ", 12, mStylesheetManager->getStylesheet("button")));
    affordableHousingRentWidget->setLayout(std::make_unique<GuiHBoxLayout>());
    housingPolicyTab->add(affordableHousingRentWidget);

    GuiWidget* apartmentBuildingRentWidget = mGui->createWithDefaultName<GuiWidget>();
    apartmentBuildingRentWidget->add(mGui->createWithDefaultName<GuiText>("Rent for an apartment: ", 12, mStylesheetManager->getStylesheet("button")));
    apartmentBuildingRentWidget->setLayout(std::make_unique<GuiHBoxLayout>());
    housingPolicyTab->add(apartmentBuildingRentWidget);

    GuiWidget* villaRentWidget = mGui->createWithDefaultName<GuiWidget>();
    villaRentWidget->add(mGui->createWithDefaultName<GuiText>("Rent for a villa: ", 12, mStylesheetManager->getStylesheet("button")));
    villaRentWidget->setLayout(std::make_unique<GuiHBoxLayout>());
    housingPolicyTab->add(villaRentWidget);

    housingPolicyTab->setLayout(std::make_unique<GuiVBoxLayout>(4.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    // Public service
    GuiWidget* publicServiceTab = mGui->createWithDefaultName<GuiWidget>(mStylesheetManager->getStylesheet("windowTabs"));
    publicServiceTab->setFixedSize(sf::Vector2f(400.0f, 100.0f));

    GuiWidget* nonQualifiedJobSalaryWidget = mGui->createWithDefaultName<GuiWidget>();
    nonQualifiedJobSalaryWidget->add(mGui->createWithDefaultName<GuiText>("Salary of a non-qualified job: ", 12, mStylesheetManager->getStylesheet("button")));
    nonQualifiedJobSalaryWidget->setLayout(std::make_unique<GuiHBoxLayout>());
    publicServiceTab->add(nonQualifiedJobSalaryWidget);

    GuiWidget* qualifiedJobSalaryWidget = mGui->createWithDefaultName<GuiWidget>();
    qualifiedJobSalaryWidget->add(mGui->createWithDefaultName<GuiText>("Salary of a qualified job: ", 12, mStylesheetManager->getStylesheet("button")));
    qualifiedJobSalaryWidget->setLayout(std::make_unique<GuiHBoxLayout>());
    publicServiceTab->add(qualifiedJobSalaryWidget);

    GuiWidget* highlyQualifiedJobSalaryWidget = mGui->createWithDefaultName<GuiWidget>();
    highlyQualifiedJobSalaryWidget->add(mGui->createWithDefaultName<GuiText>("Salary of a highly qualified job: ", 12, mStylesheetManager->getStylesheet("button")));
    highlyQualifiedJobSalaryWidget->setLayout(std::make_unique<GuiHBoxLayout>());
    publicServiceTab->add(highlyQualifiedJobSalaryWidget);

    publicServiceTab->setLayout(std::make_unique<GuiVBoxLayout>(4.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    // Tax policy
    GuiWidget* taxPolicyTab = mGui->createWithDefaultName<GuiWidget>(mStylesheetManager->getStylesheet("windowTabs"));
    taxPolicyTab->setFixedSize(sf::Vector2f(400.0f, 100.0f));

    GuiWidget* incomeTaxWidget = mGui->createWithDefaultName<GuiWidget>();
    incomeTaxWidget->add(mGui->createWithDefaultName<GuiText>("Income tax: ", 12, mStylesheetManager->getStylesheet("button")));
    incomeTaxWidget->setLayout(std::make_unique<GuiHBoxLayout>());
    taxPolicyTab->add(incomeTaxWidget);

    GuiWidget* corporateTaxWidget = mGui->createWithDefaultName<GuiWidget>();
    corporateTaxWidget->add(mGui->createWithDefaultName<GuiText>("Corporate tax: ", 12, mStylesheetManager->getStylesheet("button")));
    corporateTaxWidget->setLayout(std::make_unique<GuiHBoxLayout>());
    taxPolicyTab->add(corporateTaxWidget);

    taxPolicyTab->setLayout(std::make_unique<GuiVBoxLayout>(4.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    // Tab widget
    mTabWidget = mGui->createWithDefaultName<GuiTabWidget>();
    mTabWidget->add(laborPolicyTab);
    mTabWidget->add(housingPolicyTab);
    mTabWidget->add(publicServiceTab);
    mTabWidget->add(taxPolicyTab);
    mTabWidget->setLayout(std::make_unique<GuiOverlapLayout>());
    updateTab(0);

    // Window
    add(mTabButtonsWidget);
    add(mTabWidget);
    setPosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(0.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));
}

void PoliciesWindow::update()
{
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::GUI)
        {
            const GuiEvent& event = message.getInfo<GuiEvent>();
            switch (event.type)
            {
                case GuiEvent::Type::BUTTON_RELEASED:
                {
                    std::size_t tab = std::find(mTabButtons.begin(), mTabButtons.end(), event.widget) - mTabButtons.begin();
                    updateTab(tab);
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void PoliciesWindow::updateTab(std::size_t tab)
{
    mTabWidget->setCurrentTab(tab);
    for (GuiButton* button : mTabButtons)
        button->setState(GuiButton::State::NORMAL);
    static_cast<GuiButton*>(mTabButtonsWidget->getChildren()[tab])->setState(GuiButton::State::FORCE_PRESSED);
}
