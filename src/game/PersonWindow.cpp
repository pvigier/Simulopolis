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

#include "PersonWindow.h"
#include "resource/StylesheetManager.h"
#include "city/Person.h"
#include "gui/Gui.h"
#include "gui/GuiLabel.h"
#include "gui/GuiImage.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "util/format.h"

PersonWindow::PersonWindow(StylesheetManager* stylesheetManager, const Person& person, int year) :
    GuiWindow(person.getFullName(), stylesheetManager->getStylesheet("window")),
    mStylesheetManager(stylesheetManager), mPerson(person), mYear(year), mImage(nullptr),
    mAgeLabel(nullptr), mWorkLabel(nullptr), mShortTermGoalLabel(nullptr), mLongTermGoalLabel(nullptr),
    mEnergyLabel(nullptr), mSatietyLabel(nullptr), mHealthLabel(nullptr), mSafetyLabel(nullptr), mHappinessLabel(nullptr)
{

}

PersonWindow::~PersonWindow()
{
    //dtor
}

void PersonWindow::setUp()
{
    // Zoom
    mRenderTexture.create(96, 96);
    sf::Sprite sprite(mRenderTexture.getTexture());
    mImage = mGui->createWithDefaultName<GuiImage>(sprite);

    // Personal info
    auto infoWidget = mGui->createWithDefaultName<GuiWidget>();
    auto nameLabel = mGui->createWithDefaultName<GuiLabel>("Name: " + mPerson.getFullName(), 12, mStylesheetManager->getStylesheet("darkText"));
    mAgeLabel = mGui->createWithDefaultName<GuiLabel>("", 12, mStylesheetManager->getStylesheet("darkText"));
    mWorkLabel = mGui->createWithDefaultName<GuiLabel>("", 12, mStylesheetManager->getStylesheet("darkText"));
    mBankAccountLabel = mGui->createWithDefaultName<GuiLabel>("", 12, mStylesheetManager->getStylesheet("darkText"));
    mShortTermGoalLabel = mGui->createWithDefaultName<GuiLabel>("", 12, mStylesheetManager->getStylesheet("darkText"));
    mLongTermGoalLabel = mGui->createWithDefaultName<GuiLabel>("", 12, mStylesheetManager->getStylesheet("darkText"));
    infoWidget->add(nameLabel);
    infoWidget->add(mAgeLabel);
    infoWidget->add(mWorkLabel);
    infoWidget->add(mBankAccountLabel);
    infoWidget->add(mShortTermGoalLabel);
    infoWidget->add(mLongTermGoalLabel);
    infoWidget->setLayout(std::make_unique<GuiVBoxLayout>(3.0f));

    // Top widget
    auto topWidget = mGui->createWithDefaultName<GuiWidget>();
    topWidget->add(mImage);
    topWidget->add(infoWidget);
    topWidget->setLayout(std::make_unique<GuiHBoxLayout>(8.0f));

    // Bottom widget
    auto bottomWidget = mGui->createWithDefaultName<GuiWidget>();
    mEnergyLabel = mGui->createWithDefaultName<GuiLabel>("", 12, mStylesheetManager->getStylesheet("darkText"));
    mSatietyLabel = mGui->createWithDefaultName<GuiLabel>("", 12, mStylesheetManager->getStylesheet("darkText"));
    mHealthLabel = mGui->createWithDefaultName<GuiLabel>("", 12, mStylesheetManager->getStylesheet("darkText"));
    mSafetyLabel = mGui->createWithDefaultName<GuiLabel>("", 12, mStylesheetManager->getStylesheet("darkText"));
    mHappinessLabel = mGui->createWithDefaultName<GuiLabel>("", 12, mStylesheetManager->getStylesheet("darkText"));
    bottomWidget->add(mEnergyLabel);
    bottomWidget->add(mSatietyLabel);
    bottomWidget->add(mHealthLabel);
    bottomWidget->add(mSafetyLabel);
    bottomWidget->add(mHappinessLabel);
    bottomWidget->setLayout(std::make_unique<GuiVBoxLayout>(3.0f));

    // Window
    add(topWidget);
    add(bottomWidget);
    setOutsidePosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));
    applyStyle();
}

void PersonWindow::update()
{
    mAgeLabel->setString(format("Age: %d", mPerson.getAge(mYear)));
    mWorkLabel->setString("Work: " + mPerson.getWorkStatus());
    mBankAccountLabel->setString(format("Bank account: $%.2f", mPerson.getAccountBalance()));
    mShortTermGoalLabel->setString("Short term goal: " + mPerson.getShortTermBrain().toString());
    mLongTermGoalLabel->setString("Long term goal: " + mPerson.getLongTermBrain().toString());
    mEnergyLabel->setString(format("Energy: %.2f", mPerson.getNeed(Person::Need::ENERGY)));
    mSatietyLabel->setString(format("Satiety: %.2f", mPerson.getNeed(Person::Need::SATIETY)));
    mHealthLabel->setString(format("Health: %.2f", mPerson.getNeed(Person::Need::HEALTH)));
    mSafetyLabel->setString(format("Safety: %.2f", mPerson.getNeed(Person::Need::SAFETY)));
    mHappinessLabel->setString(format("Happiness: %.0f", 100.0f * mPerson.getNeed(Person::Need::HAPPINESS)));
}

void PersonWindow::onNewYear()
{
    ++mYear;
}

const Person& PersonWindow::getPerson() const
{
    return mPerson;
}

sf::RenderTexture& PersonWindow::getRenderTexture()
{
    return mRenderTexture;
}

sf::View PersonWindow::getView()
{
    sf::Vector2f center = mPerson.getCar().getKinematic().getPosition();
    return sf::View(center, sf::Vector2f(mRenderTexture.getSize()));
}
