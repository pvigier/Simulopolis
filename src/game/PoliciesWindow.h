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
 
#pragma once

#include "message/Mailbox.h"
#include "gui/GuiWindow.h"

class MessageBus;
class StylesheetManager;
class GuiButton;
class GuiTabWidget;
class GuiInput;
class City;

class PoliciesWindow : public GuiWindow
{
public:
    PoliciesWindow(MessageBus* messageBus, StylesheetManager* stylesheetManager, City& city);
    ~PoliciesWindow();

    virtual void setUp() override;
    virtual void tearDown() override;

    void update();

private:
    MessageBus* mMessageBus;
    StylesheetManager* mStylesheetManager;
    Mailbox mMailbox;
    GuiWidget* mTabButtonsWidget;
    std::vector<GuiButton*> mTabButtons;
    GuiTabWidget* mTabWidget;
    std::vector<GuiInput*> mInputs;
    City& mCity;

    void createLine(GuiWidget* tab, const std::string& label, const std::string& value,
        const std::string& regex, const std::string& suffix = "");
    void updateTab(std::size_t tab);
};
