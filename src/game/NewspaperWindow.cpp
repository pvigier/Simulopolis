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
#include "gui/GuiLabel.h"
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
    // Top widget
    auto topWidget = mGui->createWithDefaultName<GuiWidget>();
    topWidget->setLayout(std::make_unique<GuiVBoxLayout>(GuiLayout::HAlignment::Center, GuiLayout::VAlignment::Top, 4.0f));

    // Title
    auto nameLabel = mGui->createWithDefaultName<GuiLabel>(mNewspaper.getName(), 100,
        mStylesheetManager->getStylesheet("newspaperHeadText"));
    const Newspaper::Edition& edition = mNewspaper.getLastEdition();
    auto dateLabel = mGui->createWithDefaultName<GuiLabel>(edition.date, 16,
        mStylesheetManager->getStylesheet("newspaperText"));
    topWidget->add(nameLabel);
    topWidget->add(dateLabel);

    // Articles
    for (const Article& article : edition.articles)
    {
        auto titleText = mGui->createWithDefaultName<GuiLabel>(article.title, 18,
            mStylesheetManager->getStylesheet("newspaperTitleText"));
        auto authorText = mGui->createWithDefaultName<GuiLabel>("By " + article.author, 12,
            mStylesheetManager->getStylesheet("newspaperAuthorText"));
        auto articleText = mGui->createWithDefaultName<GuiText>(400.0f, article.body, 16,
            GuiText::Alignment::Justified, mStylesheetManager->getStylesheet("newspaperText"));
        topWidget->add(titleText);
        topWidget->add(authorText);
        topWidget->add(articleText);
    }

    // Window
    add(topWidget);
    setOutsidePosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, -32.0f, 8.0f, 8.0f}));
    applyStyle();
}

void NewspaperWindow::update()
{

}
