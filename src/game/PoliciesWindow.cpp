/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PoliciesWindow.h"
#include "message/MessageBus.h"
#include "resource/StylesheetManager.h"
#include "gui/Gui.h"
#include "gui/GuiLabel.h"
#include "gui/GuiButton.h"
#include "gui/GuiInput.h"
#include "gui/GuiTabWidget.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "gui/GuiOverlapLayout.h"
#include "gui/GuiEvent.h"
#include "city/City.h"
#include "city/Company.h"
#include "city/Work.h"
#include "util/format.h"
#include "util/regex.h"

PoliciesWindow::PoliciesWindow(MessageBus* messageBus, StylesheetManager* stylesheetManager, City& city) :
    GuiWindow("Policies", stylesheetManager->getStylesheet("window")),
    mMessageBus(messageBus), mStylesheetManager(stylesheetManager), mCity(city)
{
    mMessageBus->addMailbox(mMailbox);
}

PoliciesWindow::~PoliciesWindow()
{
    mMessageBus->removeMailbox(mMailbox);
}

void PoliciesWindow::setUp()
{
    // Tab buttons
    std::array<std::string, 4> tabNames = {"Labor policy", "Housing policy", "Public service", "Tax policy"};
    mTabButtonsWidget = mGui->createWithDefaultName<GuiWidget>();

    for (const std::string& name : tabNames)
    {
        mTabButtons.push_back(mGui->createWithDefaultName<GuiButton>(mStylesheetManager->getStylesheet("button")));
        mTabButtons.back()->add(mGui->createWithDefaultName<GuiLabel>(name, 12, mStylesheetManager->getStylesheet("darkText")));
        mTabButtons.back()->setLayout(std::make_unique<GuiHBoxLayout>(0.0f, GuiLayout::Margins{3.0f, 3.0f, 3.0f, 3.0f}));
        mTabButtons.back()->subscribe(mMailbox.getId());
        mTabButtonsWidget->add(mTabButtons.back());
    }

    mTabButtonsWidget->setLayout(std::make_unique<GuiHBoxLayout>());

    // Labor policy
    GuiWidget* laborPolicyTab = mGui->createWithDefaultName<GuiWidget>(mStylesheetManager->getStylesheet("windowTabs"));
    laborPolicyTab->setFixedInsideSize(sf::Vector2f(400.0f, 100.0f));
    laborPolicyTab->setLayout(std::make_unique<GuiVBoxLayout>(4.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    createLine(laborPolicyTab, "Weekly standard working hours: ", format("%d", mCity.getWeeklyStandardWorkingHours()), regexNumbersUntil(24 * 7));
    createLine(laborPolicyTab, "Minimum wage: $", format("%.2f", mCity.getMinimumWage()), "\\d{0,9}(\\.\\d{0,2})?");

    // Housing policy
    GuiWidget* housingPolicyTab = mGui->createWithDefaultName<GuiWidget>(mStylesheetManager->getStylesheet("windowTabs"));
    housingPolicyTab->setFixedInsideSize(sf::Vector2f(400.0f, 100.0f));
    housingPolicyTab->setLayout(std::make_unique<GuiVBoxLayout>(4.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    createLine(housingPolicyTab, "Rent for an affordable housing: $", format("%.2f", mCity.getCompany().getRent(Tile::Type::AFFORDABLE_HOUSING)), "\\d{0,9}(\\.\\d{0,2})?");
    createLine(housingPolicyTab, "Rent for an apartment: $", format("%.2f", mCity.getCompany().getRent(Tile::Type::APARTMENT_BUILDING)), "\\d{0,9}(\\.\\d{0,2})?");
    createLine(housingPolicyTab, "Rent for a villa: $", format("%.2f", mCity.getCompany().getRent(Tile::Type::VILLA)), "\\d{0,9}(\\.\\d{0,2})?");

    // Public service
    GuiWidget* publicServiceTab = mGui->createWithDefaultName<GuiWidget>(mStylesheetManager->getStylesheet("windowTabs"));
    publicServiceTab->setFixedInsideSize(sf::Vector2f(400.0f, 100.0f));
    publicServiceTab->setLayout(std::make_unique<GuiVBoxLayout>(4.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    createLine(publicServiceTab, "Salary of a non-qualified job: $", format("%.2f", mCity.getCompany().getSalary(Qualification::NON_QUALIFIED)), "\\d{0,9}(\\.\\d{0,2})?");
    createLine(publicServiceTab, "Salary of a qualified job: $", format("%.2f", mCity.getCompany().getSalary(Qualification::QUALIFIED)), "\\d{0,9}(\\.\\d{0,2})?");
    createLine(publicServiceTab, "Salary of a highly qualified job: $", format("%.2f", mCity.getCompany().getSalary(Qualification::HIGHLY_QUALIFIED)), "\\d{0,9}(\\.\\d{0,2})?");

    // Tax policy
    GuiWidget* taxPolicyTab = mGui->createWithDefaultName<GuiWidget>(mStylesheetManager->getStylesheet("windowTabs"));
    taxPolicyTab->setFixedInsideSize(sf::Vector2f(400.0f, 100.0f));
    taxPolicyTab->setLayout(std::make_unique<GuiVBoxLayout>(4.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    createLine(taxPolicyTab, "Income tax: ", format("%.0f", mCity.getIncomeTax()), "|[0-9]|[1-9][0-9]|100", "%");
    createLine(taxPolicyTab, "Corporate tax: ", format("%.0f", mCity.getCorporateTax()), "|[0-9]|[1-9][0-9]|100", "%");

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
    setOutsidePosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(0.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));
    applyStyle();
}

void PoliciesWindow::tearDown()
{
    // Labor policy
    mCity.setWeeklyStandardWorkingHours(std::stoi(mInputs[0]->getString().toAnsiString()));
    mCity.setMinimumWage(Money(std::stod(mInputs[1]->getString().toAnsiString())));

    // Housing policy
    mCity.getCompany().setRent(Tile::Type::AFFORDABLE_HOUSING, Money(std::stod(mInputs[2]->getString().toAnsiString())));
    mCity.getCompany().setRent(Tile::Type::APARTMENT_BUILDING, Money(std::stod(mInputs[3]->getString().toAnsiString())));
    mCity.getCompany().setRent(Tile::Type::VILLA, Money(std::stod(mInputs[4]->getString().toAnsiString())));

    // Public service
    mCity.getCompany().setSalary(Qualification::NON_QUALIFIED, Money(std::stod(mInputs[5]->getString().toAnsiString())));
    mCity.getCompany().setSalary(Qualification::QUALIFIED, Money(std::stod(mInputs[6]->getString().toAnsiString())));
    mCity.getCompany().setSalary(Qualification::HIGHLY_QUALIFIED, Money(std::stod(mInputs[7]->getString().toAnsiString())));

    // Tax policy
    mCity.setIncomeTax(std::stof(mInputs[8]->getString().toAnsiString()));
    mCity.setCorporateTax(std::stof(mInputs[9]->getString().toAnsiString()));
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
    widget->add(mGui->createWithDefaultName<GuiLabel>(label, 12, mStylesheetManager->getStylesheet("darkText")));

    // Input
    GuiInput* input = mGui->createWithDefaultName<GuiInput>(12, mStylesheetManager->getStylesheet("input"));
    mInputs.push_back(input);
    input->setString(value);
    input->setRegex(regex);
    widget->add(input);

    // Suffix
    if (!suffix.empty())
        widget->add(mGui->createWithDefaultName<GuiLabel>(suffix, 12, mStylesheetManager->getStylesheet("darkText")));

}

void PoliciesWindow::updateTab(std::size_t tab)
{
    mTabWidget->setCurrentTab(tab);
    for (GuiButton* button : mTabButtons)
        button->setState(GuiButton::State::NORMAL);
    static_cast<GuiButton*>(mTabButtonsWidget->getChildren()[tab])->setState(GuiButton::State::FORCE_PRESSED);
}
