#include "PoliciesWindow.h"
#include "message/MessageBus.h"
#include "resource/StylesheetManager.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiButton.h"
#include "gui/GuiInput.h"
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
    laborPolicyTab->setLayout(std::make_unique<GuiVBoxLayout>(4.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    createLine(laborPolicyTab, "Weekly standard working hours: ", "0", "|[0-9]|[1-9][0-9]|1[0-5][0-9]|16[0-8]");
    createLine(laborPolicyTab, "Minimum wage: $", "0.00", "\\d{0,9}\\.?\\d{0,2}");

    // Housing policy
    GuiWidget* housingPolicyTab = mGui->createWithDefaultName<GuiWidget>(mStylesheetManager->getStylesheet("windowTabs"));
    housingPolicyTab->setFixedSize(sf::Vector2f(400.0f, 100.0f));
    housingPolicyTab->setLayout(std::make_unique<GuiVBoxLayout>(4.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    createLine(housingPolicyTab, "Rent for an affordable housing: $", "0.00", "\\d{0,9}\\.?\\d{0,2}");
    createLine(housingPolicyTab, "Rent for an apartment: $", "0.00", "\\d{0,9}\\.?\\d{0,2}");
    createLine(housingPolicyTab, "Rent for a villa: $", "0.00", "\\d{0,9}\\.?\\d{0,2}");

    // Public service
    GuiWidget* publicServiceTab = mGui->createWithDefaultName<GuiWidget>(mStylesheetManager->getStylesheet("windowTabs"));
    publicServiceTab->setFixedSize(sf::Vector2f(400.0f, 100.0f));
    publicServiceTab->setLayout(std::make_unique<GuiVBoxLayout>(4.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    createLine(publicServiceTab, "Salary of a non-qualified job: $", "0.00", "\\d{0,9}\\.?\\d{0,2}");
    createLine(publicServiceTab, "Salary of a qualified job: $", "0.00", "\\d{0,9}\\.?\\d{0,2}");
    createLine(publicServiceTab, "Salary of a highly qualified job: $", "0.00", "\\d{0,9}\\.?\\d{0,2}");

    // Tax policy
    GuiWidget* taxPolicyTab = mGui->createWithDefaultName<GuiWidget>(mStylesheetManager->getStylesheet("windowTabs"));
    taxPolicyTab->setFixedSize(sf::Vector2f(400.0f, 100.0f));
    taxPolicyTab->setLayout(std::make_unique<GuiVBoxLayout>(4.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    createLine(taxPolicyTab, "Income tax: ", "0", "|[0-9]|[1-9][0-9]|100", "%");
    createLine(taxPolicyTab, "Corporate tax: ", "0", "|[0-9]|[1-9][0-9]|100", "%");

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

void PoliciesWindow::createLine(GuiWidget* tab, const std::string& label, const std::string& value,
    const std::string& regex, const std::string& suffix)
{
    // Line
    GuiWidget* widget = mGui->createWithDefaultName<GuiWidget>();
    widget->setLayout(std::make_unique<GuiHBoxLayout>(GuiLayout::HAlignment::Left, GuiLayout::VAlignment::Center, 2.0f));
    tab->add(widget);

    // Label
    widget->add(mGui->createWithDefaultName<GuiText>(label, 12, mStylesheetManager->getStylesheet("button")));

    // Input
    GuiWidget* inputBackground = mGui->createWithDefaultName<GuiWidget>(mStylesheetManager->getStylesheet("inputBackground"));
    GuiInput* input = mGui->createWithDefaultName<GuiInput>(12, mStylesheetManager->getStylesheet("button"));
    input->setText(value);
    input->setRegex(regex);
    inputBackground->setLayout(std::make_unique<GuiHBoxLayout>(0.0f, GuiLayout::Margins{2.0f, 2.0f, 2.0f, 2.0f}));
    inputBackground->add(input);
    widget->add(inputBackground);

    // Suffix
    if (!suffix.empty())
        widget->add(mGui->createWithDefaultName<GuiText>(suffix, 12, mStylesheetManager->getStylesheet("button")));

}

void PoliciesWindow::updateTab(std::size_t tab)
{
    mTabWidget->setCurrentTab(tab);
    for (GuiButton* button : mTabButtons)
        button->setState(GuiButton::State::NORMAL);
    static_cast<GuiButton*>(mTabButtonsWidget->getChildren()[tab])->setState(GuiButton::State::FORCE_PRESSED);
}
