#include "PersonWindow.h"
#include "resource/StylesheetManager.h"
#include "city/Person.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiImage.h"
#include "gui/GuiWindow.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "util/format.h"

PersonWindow::PersonWindow(StylesheetManager* stylesheetManager, const Person& person, int year) :
    GuiWindow(person.getFullName(), stylesheetManager->getStylesheet("window")),
    mStylesheetManager(stylesheetManager), mPerson(person), mYear(year), mImage(nullptr)
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
    auto firstNameText = mGui->createWithDefaultName<GuiText>("First name: " + mPerson.getFirstName(), 12, mStylesheetManager->getStylesheet("button"));
    auto lastNameText = mGui->createWithDefaultName<GuiText>("Last name: " + mPerson.getLastName(), 12, mStylesheetManager->getStylesheet("button"));
    auto ageText = mGui->createWithDefaultName<GuiText>(format("Age: %d", mPerson.getAge(mYear)), 12, mStylesheetManager->getStylesheet("button"));
    auto stateText = mGui->createWithDefaultName<GuiText>(format("State: %d", static_cast<int>(mPerson.getState())), 12, mStylesheetManager->getStylesheet("button"));
    infoWidget->add(firstNameText);
    infoWidget->add(lastNameText);
    infoWidget->add(ageText);
    infoWidget->add(stateText);
    infoWidget->setLayout(std::make_unique<GuiVBoxLayout>(3.0f));

    // Top widget
    auto topWidget = mGui->createWithDefaultName<GuiWidget>();
    topWidget->add(mImage);
    topWidget->add(infoWidget);
    topWidget->setLayout(std::make_unique<GuiHBoxLayout>(8.0f));

    // Bottom widget
    auto bottomWidget = mGui->createWithDefaultName<GuiWidget>();
    auto sleepText = mGui->createWithDefaultName<GuiText>(format("Sleep: %.2f", mPerson.getSleep()), 12, mStylesheetManager->getStylesheet("button"));
    auto hygieneText = mGui->createWithDefaultName<GuiText>(format("Health: %.2f", mPerson.getHealth()), 12, mStylesheetManager->getStylesheet("button"));
    auto safetyText = mGui->createWithDefaultName<GuiText>(format("Safety: %.2f", mPerson.getSafety()), 12, mStylesheetManager->getStylesheet("button"));
    auto hungerText = mGui->createWithDefaultName<GuiText>(format("Hunger: %.2f", mPerson.getHunger()), 12, mStylesheetManager->getStylesheet("button"));
    auto happinessText = mGui->createWithDefaultName<GuiText>(format("Happiness: %.2f", mPerson.getHappiness()), 12, mStylesheetManager->getStylesheet("button"));
    bottomWidget->add(sleepText);
    bottomWidget->add(hygieneText);
    bottomWidget->add(safetyText);
    bottomWidget->add(hungerText);
    bottomWidget->add(happinessText);
    bottomWidget->setLayout(std::make_unique<GuiVBoxLayout>(3.0f));

    // Window
    add(topWidget);
    add(bottomWidget);
    setPosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));
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
