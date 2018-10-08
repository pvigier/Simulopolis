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

#include "NewspaperWindow.h"
#include "resource/StylesheetManager.h"
#include "city/Newspaper.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "util/format.h"

NewspaperWindow::NewspaperWindow(StylesheetManager* stylesheetManager, const Newspaper& newspaper) :
    GuiWindow("News", stylesheetManager->getStylesheet("newspaper")),
    mStylesheetManager(stylesheetManager), mNewspaper(newspaper)
{

}

NewspaperWindow::~NewspaperWindow()
{
    //dtor
}

void NewspaperWindow::setUp()
{
    // Personal info
    auto nameText = mGui->createWithDefaultName<GuiText>(mNewspaper.getName(), 32, mStylesheetManager->getStylesheet("newspaperText"));
    auto dateText = mGui->createWithDefaultName<GuiText>("September 2000", 12, mStylesheetManager->getStylesheet("newspaperText"));

    // Top widget
    auto topWidget = mGui->createWithDefaultName<GuiWidget>();
    topWidget->add(nameText);
    topWidget->add(dateText);
    topWidget->setLayout(std::make_unique<GuiVBoxLayout>(GuiLayout::HAlignment::Center, GuiLayout::VAlignment::Top, 4.0f));

    // Bottom widget
    auto bottomWidget = mGui->createWithDefaultName<GuiWidget>();
    bottomWidget->setLayout(std::make_unique<GuiVBoxLayout>(3.0f));

    // Window
    add(topWidget);
    add(bottomWidget);
    setOutsidePosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));
    applyStyle();
}

void NewspaperWindow::update()
{

}
