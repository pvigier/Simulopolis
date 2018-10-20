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

#include <SFML/Graphics/RenderTexture.hpp>
#include "gui/GuiWindow.h"

class StylesheetManager;
class Person;
class GuiImage;
class GuiLabel;

class PersonWindow : public GuiWindow
{
public:
    PersonWindow(StylesheetManager* stylesheetManager, const Person& person, int year);
    virtual ~PersonWindow();

    virtual void setUp() override;

    void update();
    void onNewYear();

    const Person& getPerson() const;
    sf::RenderTexture& getRenderTexture();
    sf::View getView();

private:
    StylesheetManager* mStylesheetManager;
    const Person& mPerson;
    int mYear;
    GuiImage* mImage;
    GuiLabel* mAgeLabel;
    GuiLabel* mWorkLabel;
    GuiLabel* mBankAccountLabel;
    GuiLabel* mShortTermGoalLabel;
    GuiLabel* mLongTermGoalLabel;
    GuiLabel* mEnergyLabel;
    GuiLabel* mSatietyLabel;
    GuiLabel* mHealthLabel;
    GuiLabel* mSafetyLabel;
    GuiLabel* mHappinessLabel;
    sf::RenderTexture mRenderTexture;
};
