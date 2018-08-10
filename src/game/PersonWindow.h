#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include "gui/GuiWindow.h"

class StylesheetManager;
class Person;
class GuiImage;
class GuiText;

class PersonWindow : public GuiWindow
{
public:
    PersonWindow(StylesheetManager* stylesheetManager, const Person& person, int year);
    ~PersonWindow();

    virtual void setUp() override;

    void update();
    void onNewYear();

    sf::RenderTexture& getRenderTexture();
    sf::View getView();

private:
    StylesheetManager* mStylesheetManager;
    const Person& mPerson;
    int mYear;
    GuiImage* mImage;
    GuiText* mAgeText;
    GuiText* mWorkText;
    GuiText* mShortTermGoalText;
    GuiText* mLongTermGoalText;
    GuiText* mSleepText;
    GuiText* mHealthText;
    GuiText* mSafetyText;
    GuiText* mHungerText;
    GuiText* mHappinessText;
    sf::RenderTexture mRenderTexture;
};
