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

class StylesheetManager;
class GuiTable;
class City;
class Person;

class CitizensWindow : public GuiWindow
{
public:
    CitizensWindow(Id listenerId, MessageBus* messageBus, StylesheetManager* stylesheetManager, City& city);
    ~CitizensWindow();

    virtual void setUp() override;

    void update();

private:
    Id mListenerId;
    MessageBus* mMessageBus;
    StylesheetManager* mStylesheetManager;
    Mailbox mMailbox;
    City& mCity;
    std::vector<Person*> mCitizens;
    GuiTable* mTable;

    void addCitizen(Person* person);
    void removeCitizen(Person* person);
};
